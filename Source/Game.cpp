#include "Game.hpp"
#include <algorithm>
#include <iostream>
#include "WindowConfig.hpp"
#include <ranges>
#include <vector>
#include <cmath>
#include <fstream>

Game::Game()
    : window(title, screenWidth, screenHeight)
{
    gameState = State::STARTSCREEN;
    score = 0;
    shootTimer = 0.0f;
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

void Game::Start()
{
    SpawnWalls();
    resources.Load();
    player.RePosition();
    SpawnAliens();
    Background newBackground;
    newBackground.Initialize(backgroundSpeed);
    background = newBackground;
    score = 0;
    gameState = State::GAMEPLAY;
}

void Game::End()
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Continue() noexcept { gameState = State::STARTSCREEN; }
void Game::Launch() { resources.Load(); }

void Game::Update()
{
    switch (gameState)
    {
    case State::STARTSCREEN:
        if (IsKeyReleased(KEY_SPACE)) { Start(); }
        break;

    case State::GAMEPLAY:
        UpdatePlayerInput();
        UpdateAliens();
        UpdateProjectiles();
        DetectCollisions();
        UpdateWalls();
        UpdateBackground();
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

void Game::Render()
{
    switch (gameState)
    {
    case State::STARTSCREEN:
        DrawText("SPACE INVADERS", 200, 100, startScreenTitleFontSize, YELLOW);
        DrawText("PRESS SPACE TO BEGIN", 200, 350, startScreenSubtitleFontSize, YELLOW);
        break;

    case State::GAMEPLAY:
        background.Render();
        DrawText(TextFormat("Score: %i", score), 50, 20, gameplayScoreFontSize, YELLOW);
        DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, gameplayLivesFontSize, YELLOW);
        player.Render();

        for (int i = 0; i < Projectiles.size(); i++) {
            Projectiles[i].Render(resources.laserTexture);
        }

        for (int i = 0; i < Walls.size(); i++) {
            Walls[i].Render();
        }

        for (int i = 0; i < Aliens.size(); i++) {
            Aliens[i].Render();
        }
        break;

    case State::ENDSCREEN:
        DrawText("PRESS ENTER TO CONTINUE", textBoxX, 200, endScreenFontSize, YELLOW);
        break;

    default:
        break;
    }
}

void Game::UpdatePlayerInput()
{
    if (IsKeyReleased(KEY_Q)) { End(); }
    player.Update();
    if (IsKeyPressed(KEY_SPACE)) { Projectiles.push_back(player.Shoot()); }
}

void Game::SpawnAliens()
{
    if (Aliens.empty()) 
    {
        for (int row = 0; row < alienFormationHeight; row++)
        {
            for (int col = 0; col < alienFormationWidth; col++)
            {
                Alien newAlien;
                newAlien.SetPosition({ static_cast<float>(alienFormationX + 450 + (col * alienSpacing)),
                                       static_cast<float>(alienFormationY + (row * alienSpacing)) });
                Aliens.push_back(newAlien);
                Alien::IncrementInstanceCount();
            }
        }
    }
}

void Game::UpdateAliens()
{
    RandomizeAlienShot();

    for (auto& alien : Aliens)
    {
        alien.Update();
        if (alien.GetPosition().y > screenHeight - player.GetPlayerBaseHeight())
        {
            End();
        }
    }
    RemoveInactiveAliens();
    if (Aliens.empty()) { SpawnAliens(); }
}

void Game::RandomizeAlienShot()
{
    shootTimer += GetFrameTime();

    if (shootTimer >= alienShootInterval && !Aliens.empty())
    {
        const int randomAlienIndex = rand() % Aliens.size();
        auto& randomAlien = Aliens[randomAlienIndex];

        if (randomAlien.IsActive()) {
            Projectiles.push_back(randomAlien.Shoot());
            std::cout << "Alien at index " << randomAlienIndex << " fired a projectile!" << std::endl;
        }
        shootTimer = 0.0f;
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

void Game::SpawnWalls()
{
    constexpr auto window_width = static_cast<float>(screenWidth);
    constexpr auto window_height = static_cast<float>(screenHeight);
    constexpr auto wall_distance = window_width / (defaultWallCount + 1);

    Walls.clear();
    Walls.reserve(defaultWallCount);

    for (const int i : std::views::iota(1, defaultWallCount + 1)) {
        Walls.emplace_back(); // Construct Wall in place
        Walls.back().SetPosition({ wall_distance * static_cast<float>(i), window_height - 250.0f });
        Wall::IncrementInstanceCount();
    }
}

void Game::UpdateWalls()
{
    for (auto& wall : Walls) 
    {
        wall.Update();
    }
    RemoveInactiveWalls();
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

void Game::UpdateProjectiles()
{
    for (auto& projectile : Projectiles)
    {
        projectile.Update();
    }
    std::erase_if(Projectiles, [](const Projectile& p) { return !p.active; });
}

void Game::DetectCollisions()
{
    for (auto& projectile : Projectiles) 
    {
        if (projectile.type == EntityType::PLAYER_PROJECTILE)     { CheckPlayerCollision(projectile); }
        else if (projectile.type == EntityType::ENEMY_PROJECTILE) { CheckEnemyCollision(projectile); }
        CheckWallCollisions(projectile);
    }
}

void Game::CheckPlayerCollision(Projectile& projectile)
{
    for (auto& alien : Aliens) 
    {
        if (alien.IsActive() && CheckCollision(alien.GetPosition(), alien.GetRadius(), projectile.lineStart, projectile.lineEnd)) {
            projectile.active = false;
            alien.SetActive(false);
            score += 100;
            std::cout << "Alien " << &alien << " hit by player projectile!" << std::endl;
        }
    }
}

void Game::CheckEnemyCollision(Projectile& projectile)
{
    if (CheckCollision({ player.GetXPosition(), screenHeight - player.GetPlayerBaseHeight() }, player.GetRadius(), projectile.lineStart, projectile.lineEnd)) 
    {
        projectile.active = false;
        player.SetLives(player.GetLives() - 1);
        std::cout << "Player hit by alien projectile!" << std::endl;

        if (player.GetLives() <= 0) { End(); }
    }
}

void Game::CheckWallCollisions(Projectile& projectile)
{
    for (auto& wall : Walls) 
    {
        if (CheckCollision(wall.GetPosition(), wall.GetRadius(), projectile.lineStart, projectile.lineEnd)) 
        {
            projectile.active = false;
            wall.SetHealth(wall.GetHealth() - 1);
            std::cout << "Wall hit! Remaining health: " << wall.GetHealth() << std::endl;

            if (wall.GetHealth() <= 0) {
                wall.SetActive(false);
                std::cout << "Wall destroyed!" << std::endl;
            }
        }
    }
}

bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
{
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

void Game::UpdateBackground()
{
    const Vector2 playerPos = { player.GetXPosition(), player.GetPlayerBaseHeight() };
    const Vector2 cornerPos = { 0.0f, player.GetPlayerBaseHeight() };
    const float offset = lineLength(playerPos, cornerPos) * -1.0f;

    background.Update(offset / backgroundSpeed);
}

bool Game::CheckNewHighScore() {
    return score > Leaderboard.back().score;
}