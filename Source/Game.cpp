#include "Config.hpp"
#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>

Game::Game()
    : window()
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

namespace
{
    bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) noexcept
    {
        const float dx = circlePos.x - point.x;
        const float dy = circlePos.y - point.y;
        const float squaredDistance = dx * dx + dy * dy;
        return squaredDistance < radius * radius;
    }
}

void Game::Start()
{
    SpawnWalls();
    resources.Load();       //ToDo: Remove
    player.RePosition();
    SpawnAliens();
    Background newBackground;
    newBackground.Initialize(backgroundSpeed);
    background = newBackground;
    score = 0;
    gameState = State::GAMEPLAY;
}

void Game::End() noexcept
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Continue() noexcept { gameState = State::STARTSCREEN; }
void Game::Launch() { resources.Load(); }   //ToDo: Remove

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
        if (IsKeyReleased(KEY_ENTER)) { Continue(); }
        break;
    }
}

void Game::Render()
{
    switch (gameState)
    {
    case State::STARTSCREEN:
        RenderStartScreen();
        break;

    case State::GAMEPLAY:
        RenderGameplay();
        break;

    case State::ENDSCREEN:
        DrawText(continueMessage.data(), textBoxX, 200, endScreenFontSize, YELLOW);
        break;
    }
}

void Game::RenderStartScreen() noexcept
{
    DrawText(title.data(), 200, 100, startScreenTitleFontSize, YELLOW);
    DrawText(beginMessage.data(), 200, 350, startScreenSubtitleFontSize, YELLOW);
}

void Game::RenderGameplay()
{
    background.Render();
    DrawText(TextFormat("Score: %i", score), 50, 20, gameplayScoreFontSize, YELLOW);
    DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, gameplayLivesFontSize, YELLOW);
    player.Render();

    for (const auto& projectile : Projectiles)
    {
        projectile.Render(resources.laserTexture);
    }

    for (const auto& wall : Walls) 
    {
        wall.Render();
    }

    for (const auto& alien : Aliens) 
    {
        alien.Render();
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
    TriggerAlienShot();
    for (auto& alien : Aliens)
    {
        alien.Update();
        if (alien.GetPosition().y > screenHeight - player.GetPlayerBaseHeight()) { End(); }
    }
    RemoveInactiveAliens();
    if (Aliens.empty()) { SpawnAliens(); }
}

void Game::TriggerAlienShot()
{
    shootTimer += GetFrameTime();
    if (shootTimer >= alienShootInterval && !Aliens.empty())
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, Aliens.size() - 1);

        const int randomAlienIndex = dist(gen);
        assert(randomAlienIndex >= 0 && randomAlienIndex < Aliens.size());
        auto& randomAlien = Aliens[randomAlienIndex];

        if (randomAlien.IsActive())
        {
            Projectiles.push_back(randomAlien.Shoot());
            std::cout << "Alien at index " << randomAlienIndex << " fired a projectile!" << std::endl;
        }
        shootTimer = 0.0f;
    }
}

void Game::RemoveInactiveAliens()
{
    auto it = std::remove_if(Aliens.begin(), Aliens.end(), [](const Alien& alien)
        {
            return !alien.IsActive();
        });

    for (auto removedIt = it; removedIt != Aliens.end(); ++removedIt) 
    {
        std::cout << "Removing Alien with ID: " << &(*removedIt) << std::endl;
        Alien::DecrementInstanceCount();
    }
    Aliens.erase(it, Aliens.end());
}

void Game::SpawnWalls()
{
    constexpr auto window_width = static_cast<float>(screenWidth);
    constexpr auto window_height = static_cast<float>(screenHeight);
    constexpr auto wall_distance = window_width / (defaultWallCount + 1);

    Walls.clear();
    Walls.reserve(defaultWallCount);

    for (const int i : std::views::iota(1, defaultWallCount + 1)) 
    {
        Walls.emplace_back();
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
    auto it = std::remove_if(Walls.begin(), Walls.end(), [](const Wall& wall) noexcept
        {
            return !wall.IsActive();
        });
    for (auto removedIt = it; removedIt != Walls.end(); ++removedIt) 
    {
        Wall::DecrementInstanceCount();
    }
    Walls.erase(it, Walls.end());
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
        if (alien.IsActive() && CheckCollision(alien.GetPosition(), alien.GetRadius(), projectile.lineStart, projectile.lineEnd))
        {
            projectile.active = false;
            alien.SetActive(false);
            score += 100;
            std::cout << "Alien " << &alien << " hit by player projectile!" << std::endl;
        }
    }
}

void Game::CheckEnemyCollision(Projectile& projectile)
{
    if (CheckCollision({ player.GetXPosition(), screenHeight - player.GetPlayerBaseHeight() },
        player.GetRadius(), projectile.lineStart, projectile.lineEnd)) 
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
    const auto dx = lineEnd.x - lineStart.x;
    const auto dy = lineEnd.y - lineStart.y;
    const auto lineLengthSquared = dx * dx + dy * dy;

    const auto dotP = ((circlePos.x - lineStart.x) * dx + (circlePos.y - lineStart.y) * dy) / lineLengthSquared;
    const auto closestX = std::clamp(lineStart.x + dotP * dx, std::min(lineStart.x, lineEnd.x), std::max(lineStart.x, lineEnd.x));
    const auto closestY = std::clamp(lineStart.y + dotP * dy, std::min(lineStart.y, lineEnd.y), std::max(lineStart.y, lineEnd.y));
    const auto distanceSquared = (closestX - circlePos.x) * (closestX - circlePos.x) +
                                  (closestY - circlePos.y) * (closestY - circlePos.y);

    return distanceSquared <= circleRadius * circleRadius;
}

void Game::UpdateBackground()
{
    const auto playerBaseHeight = player.GetPlayerBaseHeight();
    const Vector2 playerPosition = { player.GetXPosition(), playerBaseHeight };
    const Vector2 screenCorner = { 0.0f, playerBaseHeight };
    const auto dx = screenCorner.x - playerPosition.x;
    const auto dy = screenCorner.y - playerPosition.y;
    const auto backgroundOffset = std::sqrt(dx * dx + dy * dy) * -1.0f;
    background.Update(backgroundOffset / backgroundSpeed);
}

bool Game::CheckNewHighScore() noexcept
{
    return score > Leaderboard.back().score;
}