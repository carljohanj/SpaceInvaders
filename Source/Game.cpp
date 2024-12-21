#include "Game.hpp"
#include <iostream>
#include "WindowConfig.hpp"
#include <vector>
#include <cmath>
#include <fstream>

Game::Game()
    : window("Space Invaders", screenWidth, screenHeight)
{
    gameState = State::STARTSCREEN;
    score = 0;
    shootTimer = 0.0f;
    wallCount = 5;
}

void Game::Run()
{
    Start();
    while (!WindowShouldClose())
    {
        Update();
        window.BeginDrawing();
        ClearBackground(BLACK);
        Render();
        window.EndDrawing();
    }
    CloseAudioDevice();
}

// MATH FUNCTIONS
float lineLength(Vector2 A, Vector2 B)
{
    return sqrtf(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point)
{
    float distanceToCentre = lineLength(circlePos, point);
    return distanceToCentre < radius;
}

// Game Method Implementations
void Game::Start() 
{
    const auto window_width = (float)GetScreenWidth();
    const auto window_height = (float)GetScreenHeight();
    const auto wall_distance = window_width / (wallCount + 1);

    // Create walls at the start
    for (int i = 0; i < wallCount; i++) {
        Wall newWall;
        newWall.SetPosition({ wall_distance * (i + 1), window_height - 250 });
        Walls.push_back(newWall);
        Wall::IncrementInstanceCount();
    }

    resources.Load();  // Load resources (textures, fonts, etc.)
    player.RePosition();
    SpawnAliens();
    Background newBackground;
    newBackground.Initialize(600);
    background = newBackground;

    score = 0;
    gameState = State::GAMEPLAY;
}

void Game::End() {
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Continue() {
    gameState = State::STARTSCREEN;
}

void Game::Launch() {
    resources.Load();
}

void Game::Update() {
    switch (gameState) {
    case State::STARTSCREEN:
        if (IsKeyReleased(KEY_SPACE)) {
            Start();
        }
        break;

    case State::GAMEPLAY:
        if (IsKeyReleased(KEY_Q)) {
            End();
        }

        // Update player
        player.Update();

        if (IsKeyPressed(KEY_SPACE)) { Projectiles.push_back(player.Shoot()); }

        shootTimer += GetFrameTime();
        if (shootTimer >= 1.0f)
        {
            if (!Aliens.empty())
            {
                int randomAlienIndex = rand() % Aliens.size();
                if (Aliens[randomAlienIndex].IsActive()) {
                    Projectiles.push_back(Aliens[randomAlienIndex].Shoot());
                    std::cout << "Alien at index " << randomAlienIndex << " fired a projectile!" << std::endl;
                }
            }
            shootTimer = 0.0f;
        }

        // Update aliens
        for (int i = 0; i < Aliens.size(); i++) {
            Aliens[i].Update();

            if (Aliens[i].GetPosition().y > GetScreenHeight() - player.GetPlayerBaseHeight()) {
                End();
            }
        }

        RemoveInactiveAliens();
        if (Aliens.empty()) { SpawnAliens(); }

        // Update projectiles
        for (int i = 0; i < Projectiles.size(); i++) {
            Projectiles[i].Update();
            if (!Projectiles[i].active) {
                Projectiles.erase(Projectiles.begin() + i);
                i--;
            }
        }

        // Check collisions
        for (int i = 0; i < Projectiles.size(); i++) {
            if (Projectiles[i].type == EntityType::PLAYER_PROJECTILE) {
                for (int a = 0; a < Aliens.size(); a++) {
                    if (Aliens[a].IsActive() && CheckCollision(Aliens[a].GetPosition(), Aliens[a].GetRadius(), Projectiles[i].lineStart, Projectiles[i].lineEnd)) {
                        Projectiles[i].active = false;  // Deactivate the projectile
                        Aliens[a].SetActive(false);  // Deactivate the alien
                        score += 100;  // Increase score
                        std::cout << "Alien " << &Aliens[a] << " hit by player projectile!" << std::endl;  // Print ID of the alien hit
                    }
                }
            }
            else if (Projectiles[i].type == EntityType::ENEMY_PROJECTILE) {
                if (CheckCollision({ player.GetXPosition(), GetScreenHeight() - player.GetPlayerBaseHeight() }, player.GetRadius(), Projectiles[i].lineStart, Projectiles[i].lineEnd)) {
                    Projectiles[i].active = false;
                    player.SetLives(player.GetLives() - 1);
                    std::cout << "Player hit by alien projectile!" << std::endl;

                    if (player.GetLives() <= 0) {
                        End();
                    }
                }
            }

            // Check collisions with walls
            for (int w = 0; w < Walls.size(); w++) {
                if (CheckCollision(Walls[w].GetPosition(), Walls[w].GetRadius(), Projectiles[i].lineStart, Projectiles[i].lineEnd)) {
                    Projectiles[i].active = false;
                    Walls[w].SetHealth(Walls[w].GetHealth() - 1);
                    std::cout << "Wall hit! Remaining health: " << Walls[w].GetHealth() << std::endl;

                    if (Walls[w].GetHealth() <= 0) 
                    {
                        Walls[w].SetActive(false);
                        std::cout << "Wall destroyed!" << std::endl;
                    }
                }
            }
        }

        RemoveInactiveWalls();

        // Update walls
        for (int i = 0; i < Walls.size(); i++) {
            Walls[i].Update();
        }

        // Update background
        playerPos = { player.GetXPosition(), (float)player.GetPlayerBaseHeight() };
        cornerPos = { 0, (float)player.GetPlayerBaseHeight() };
        offset = lineLength(playerPos, cornerPos) * -1;
        background.Update(offset / 15);

        break;

    case State::ENDSCREEN:
        if (IsKeyReleased(KEY_ENTER)) {
            Continue();
        }
        break;

    default:
        break;
    }
}

void Game::Render() {
    switch (gameState) {
    case State::STARTSCREEN:
        DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
        break;

    case State::GAMEPLAY:
        background.Render();
        DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
        DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, 40, YELLOW);
        player.Render();

        for (int i = 0; i < Projectiles.size(); i++) {
            Projectiles[i].Render(resources.laserTexture);
        }

        // Render Walls using their own texture
        for (int i = 0; i < Walls.size(); i++) {
            Walls[i].Render();
        }

        for (int i = 0; i < Aliens.size(); i++) {
            Aliens[i].Render();
        }
        break;

    case State::ENDSCREEN:
        DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
        break;

    default:
        break;
    }
}

void Game::SpawnAliens() 
{
    if (Aliens.empty()) {
        for (int row = 0; row < formationHeight; row++) {
            for (int col = 0; col < formationWidth; col++) {
                Alien newAlien;
                newAlien.SetPosition({ static_cast<float>(formationX + 450 + (col * alienSpacing)),
                                       static_cast<float>(formationY + (row * alienSpacing)) });
                Aliens.push_back(newAlien);
                Alien::IncrementInstanceCount();
            }
        }
    }
}

void Game::RemoveInactiveAliens() 
{
    for (int i = 0; i < Aliens.size(); i++) 
    {
        if (!Aliens[i].IsActive()) {
            std::cout << "Removing Alien with ID: " << &Aliens[i] << std::endl;
            Aliens.erase(Aliens.begin() + i);
            Alien::DecrementInstanceCount();
            i--;
        }
    }
}

void Game::RemoveInactiveWalls()
{
    for (int i = 0; i < Walls.size(); i++) 
    {
        if (!Walls[i].IsActive()) {
            Walls.erase(Walls.begin() + i);
            Wall::DecrementInstanceCount();
            i--;
        }
    }
}

bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd) {
    Vector2 A = lineStart;
    Vector2 B = lineEnd;
    Vector2 C = circlePos;

    float length = lineLength(A, B);
    float dotP = (((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2);

    float closestX = A.x + (dotP * (B.x - A.x));
    float closestY = A.y + (dotP * (B.y - A.y));

    if (closestX < std::min(A.x, B.x) || closestX > std::max(A.x, B.x) ||
        closestY < std::min(A.y, B.y) || closestY > std::max(A.y, B.y)) {
        return false;
    }

    float distance = lineLength({ closestX, closestY }, C);
    return distance <= circleRadius;
}

bool Game::CheckNewHighScore() {
    return score > Leaderboard.back().score;
}