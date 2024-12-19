#include "Game.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// MATH FUNCTIONS
float lineLength(Vector2 A, Vector2 B) {
    return sqrtf(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) {
    float distanceToCentre = lineLength(circlePos, point);
    return distanceToCentre < radius;
}

// Game Method Implementations
void Game::Start() {
    float window_width = (float)GetScreenWidth();
    float window_height = (float)GetScreenHeight();
    float wall_distance = window_width / (wallCount + 1);

    for (int i = 0; i < wallCount; i++) {
        Wall newWall;
        newWall.position.y = window_height - 250;
        newWall.position.x = wall_distance * (i + 1);
        Walls.push_back(newWall);
    }

    player.Initialize();

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

void Game::Continue() 
{
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

        player.Update();

        // Player shoots a projectile
        if (IsKeyPressed(KEY_SPACE)) {
            float window_height = (float)GetScreenHeight();
            Projectile newProjectile;
            newProjectile.position.x = player.x_pos;
            newProjectile.position.y = window_height - 130;
            newProjectile.type = EntityType::PLAYER_PROJECTILE;
            newProjectile.active = true;
            Projectiles.push_back(newProjectile);
        }

        // Update all projectiles
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
                    if (Aliens[a].active && CheckCollision(Aliens[a].position, Aliens[a].radius, Projectiles[i].lineStart, Projectiles[i].lineEnd)) {
                        std::cout << "Alien hit by projectile!" << std::endl;
                        Projectiles[i].active = false;
                        Aliens[a].active = false;
                        score += 100;
                    }
                }
            }
        }

        // Remove inactive aliens
        for (int i = 0; i < Aliens.size(); i++) {
            if (!Aliens[i].active) {
                Aliens.erase(Aliens.begin() + i);
                i--;
            }
        }

        // Update aliens
        for (int i = 0; i < Aliens.size(); i++) {
            Aliens[i].Update();
            if (Aliens[i].position.y > GetScreenHeight() - player.player_base_height) {
                End();
            }
        }

        if (player.lives < 1) {
            End();
        }

        if (Aliens.empty()) {
            SpawnAliens();
        }

        playerPos = { player.x_pos, (float)player.player_base_height };
        cornerPos = { 0, (float)player.player_base_height };
        offset = lineLength(playerPos, cornerPos) * -1;
        background.Update(offset / 15);

        for (int i = 0; i < Walls.size(); i++) {
            Walls[i].Update();
        }

        break;

    case State::ENDSCREEN:
        if (IsKeyReleased(KEY_ENTER) && !newHighScore) {
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
        DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);
        player.Render(resources.shipTextures[player.activeTexture]);

        for (int i = 0; i < Projectiles.size(); i++) {
            Projectiles[i].Render(resources.laserTexture);
        }

        for (int i = 0; i < Walls.size(); i++) {
            Walls[i].Render(resources.barrierTexture);
        }

        for (int i = 0; i < Aliens.size(); i++) {
            Aliens[i].Render(resources.alienTexture);
        }
        break;

    case State::ENDSCREEN:
        DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
        break;

    default:
        break;
    }
}

void Game::SpawnAliens() {
    for (int row = 0; row < formationHeight; row++) {
        for (int col = 0; col < formationWidth; col++) {
            Alien newAlien;
            newAlien.active = true;
            newAlien.position.x = formationX + 450 + (col * alienSpacing);
            newAlien.position.y = formationY + (row * alienSpacing);
            Aliens.push_back(newAlien);
        }
    }
}

bool Game::CheckNewHighScore() {
    if (score > Leaderboard[4].score) {
        return true;
    }
    return false;
}

void Game::InsertNewHighScore(std::string name) {
    PlayerData newData;
    newData.name = name;
    newData.score = score;

    for (int i = 0; i < Leaderboard.size(); i++) {
        if (newData.score > Leaderboard[i].score) {
            Leaderboard.insert(Leaderboard.begin() + i, newData);
            Leaderboard.pop_back();
            break;
        }
    }
}

// Projectile Implementations
void Projectile::Update() {
    position.y -= speed;
    lineStart.y = position.y - 15;
    lineEnd.y = position.y + 15;
    lineStart.x = position.x;
    lineEnd.x = position.x;

    if (position.y < 0 || position.y > 1500) {
        active = false;
    }
}

void Projectile::Render(Texture2D texture) {
    DrawTexturePro(texture,
        { 0, 0, 176, 176 },
        { position.x, position.y, 50, 50 },
        { 25 , 25 },
        0,
        WHITE);
}

// Wall Implementations
void Wall::Update() {
    if (health < 1) {
        active = false;
    }
}

void Wall::Render(Texture2D texture) {
    DrawTexturePro(texture,
        { 0, 0, 704, 704 },
        { position.x, position.y, 200, 200 },
        { 100 , 100 },
        0,
        WHITE);
    DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

// Alien Implementations
void Alien::Update() {
    if (moveRight) {
        position.x += speed;
        if (position.x >= GetScreenWidth()) {
            moveRight = false;
            position.y += 50;
        }
    }
    else {
        position.x -= speed;
        if (position.x <= 0) {
            moveRight = true;
            position.y += 50;
        }
    }
}

void Alien::Render(Texture2D texture) {
    DrawTexturePro(texture,
        { 0, 0, 352, 352 },
        { position.x, position.y, 100, 100 },
        { 50 , 50 },
        0,
        WHITE);
}

// Background Implementations
void Background::Initialize(int starAmount) {
    for (int i = 0; i < starAmount; i++) {
        Star newStar;
        newStar.initPosition.x = GetRandomValue(-150, GetScreenWidth() + 150);
        newStar.initPosition.y = GetRandomValue(0, GetScreenHeight());
        newStar.color = SKYBLUE;
        newStar.size = GetRandomValue(1, 4) / 2;
        Stars.push_back(newStar);
    }
}

void Background::Update(float offset) {
    for (auto& star : Stars) {
        star.Update(offset);
    }
}

void Background::Render() {
    for (const auto& star : Stars) {
        star.Render();
    }
}

void Star::Update(float starOffset) {
    position.x = initPosition.x + starOffset;
    position.y = initPosition.y;
}

void Star::Render() const {
    DrawCircle((int)position.x, (int)position.y, size, color);
}

bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd) {
    Vector2 A = lineStart;
    Vector2 B = lineEnd;
    Vector2 C = circlePos;

    // Calculate the length of the line
    const float length = lineLength(A, B);

    // Calculate the dot product
    const float dotP = (((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2);

    // Find the closest point on the line
    float closestX = A.x + (dotP * (B.x - A.x));
    float closestY = A.y + (dotP * (B.y - A.y));

    // Ensure the closest point lies on the segment
    if (closestX < std::min(A.x, B.x) || closestX > std::max(A.x, B.x) ||
        closestY < std::min(A.y, B.y) || closestY > std::max(A.y, B.y)) {
        return false;
    }

    // Check if the closest point is within the circle
    float distance = lineLength({ closestX, closestY }, C);
    return distance <= circleRadius;
}