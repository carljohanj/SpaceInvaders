#include "Config.hpp"
#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include "ImplWrapper.hpp"
#include <random>
#include <ranges>

struct Game::Private
{
    State gameState = State::STARTSCREEN;
    int score = 0;
    float shootTimer = 0;
    bool newHighScore = false;

    GameWindow window;
    Background background;
    Player player;
    std::vector<Alien> Aliens;
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;

    void Start();
    void Update();
    void Render() noexcept;
    void End() noexcept;
    void Continue() noexcept;
    void ResetGameState() noexcept;
    void ResetAliens();
    void RenderGameplay() noexcept;
    void RenderStartScreen() const noexcept;
    void RenderGameOverScreen() const noexcept;
    void RenderHUD() const noexcept;
    void UpdatePlayerInput();
    void RenderAliens() noexcept;
    void UpdateAliens();
    void TriggerAlienShot();
    void RemoveInactiveAliens();
    void ResetWalls();
    void RenderWalls() noexcept;
    void UpdateWalls();
    void RemoveInactiveWalls();
    void RenderProjectiles() const noexcept;
    void UpdateProjectiles();
    void DetectCollisions();
    void CheckPlayerCollision(Projectile& projectile);
    void CheckEnemyCollision(Projectile& projectile);
    void CheckWallCollisions(Projectile& projectile);
    bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd);
    void UpdateBackground();
    bool CheckNewHighScore() noexcept;
};

Game::Game()
{
    impl.initialize<Private>();
}

void Game::Run()
{
    auto* game = impl.get<Private>();
    assert(game != nullptr);

    game->Start();
    while (!WindowShouldClose())
    {
        game->Update();
        game->window.BeginDrawing();
        ClearBackground(BLACK);
        game->Render();
        game->window.EndDrawing();
    }
}

void Game::Private::Start()
{
    ResetWalls();
    player.Reset();
    ResetAliens();
    background.Reset();
    ResetGameState();
}

void Game::Private::Update()
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

void Game::Private::Render() noexcept
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
        RenderGameOverScreen();
        break;
    }
}

void Game::Private::End() noexcept
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
    newHighScore = CheckNewHighScore();
    gameState = State::ENDSCREEN;
}

void Game::Private::Continue() noexcept { gameState = State::STARTSCREEN; }

void Game::Private::ResetGameState() noexcept
{
    score = 0;
    gameState = State::GAMEPLAY;
}

void Game::Private::RenderGameplay() noexcept
{
    background.Render();
    RenderHUD();
    player.Render();
    RenderProjectiles();
    RenderWalls();
    RenderAliens();
}

void Game::Private::RenderStartScreen() const noexcept
{
    DrawText(Config::title.data(), 200, 100, Config::startScreenTitleFontSize, YELLOW);
    DrawText(Config::beginMessage.data(), 200, 350, Config::startScreenSubtitleFontSize, YELLOW);
}

void Game::Private::RenderGameOverScreen() const noexcept
{
    DrawText(Config::continueMessage.data(), Config::textBoxX, 200, Config::endScreenFontSize, YELLOW);
}

void Game::Private::RenderHUD() const noexcept
{
    DrawText(TextFormat("Score: %i", score), 50, 20, Config::gameplayScoreFontSize, YELLOW);
    DrawText(TextFormat("Lives: %i", player.GetLives()), 50, 70, Config::gameplayLivesFontSize, YELLOW);
}

void Game::Private::UpdatePlayerInput()
{
    if (IsKeyReleased(KEY_Q)) { End(); }
    player.Update();
    if (IsKeyPressed(KEY_SPACE)) { Projectiles.push_back(player.Shoot()); }
}

void Game::Private::ResetAliens()
{
    if (Aliens.empty())
    {
        for (int row = 0; row < Config::alienFormationHeight; row++)
        {
            for (int col = 0; col < Config::alienFormationWidth; col++)
            {
                Alien newAlien;
                newAlien.SetPosition({ static_cast<float>(Config::alienFormationX + 450 + (col * Config::alienSpacing)),
                                       static_cast<float>(Config::alienFormationY + (row * Config::alienSpacing)) });
                Aliens.push_back(std::move(newAlien));
            }
        }
    }
}

void Game::Private::RenderAliens() noexcept
{
    for (const auto& alien : Aliens)
    {
        alien.Render();
    }
}

void Game::Private::UpdateAliens()
{
    RemoveInactiveAliens();
    TriggerAlienShot();
    for (auto& alien : Aliens)
    {
        alien.Update();
        if (alien.GetPosition().y > Config::screenHeight - player.GetPlayerBaseHeight()) { End(); }
    }
    if (Aliens.empty()) { ResetAliens(); }
}

void Game::Private::TriggerAlienShot()
{
    shootTimer += GetFrameTime();
    if (shootTimer >= Config::alienShootInterval && !Aliens.empty())
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, Aliens.size() - 1);

        const int randomAlienIndex = dist(gen);
        assert(randomAlienIndex >= 0 && randomAlienIndex < Aliens.size());
        auto& randomAlien = Aliens[randomAlienIndex];

        if (randomAlien.IsActive()) { Projectiles.push_back(randomAlien.Shoot()); }
        shootTimer = 0.0f;
    }
}

void Game::Private::RemoveInactiveAliens()
{
    auto it = std::remove_if(Aliens.begin(), Aliens.end(), [](const Alien& alien) noexcept
        {
            return !alien.IsActive();
        });

    Aliens.erase(it, Aliens.end());
}

void Game::Private::ResetWalls()
{
    constexpr auto window_width = static_cast<float>(Config::screenWidth);
    constexpr auto window_height = static_cast<float>(Config::screenHeight);
    constexpr auto wall_distance = window_width / (Config::defaultWallCount + 1);

    Walls.clear();
    Walls.reserve(Config::defaultWallCount);

    for (const int i : std::views::iota(1, Config::defaultWallCount + 1))
    {
        Walls.emplace_back();
        Walls.back().SetPosition({ wall_distance * static_cast<float>(i), window_height - 250.0f });
    }
}

void Game::Private::RenderWalls() noexcept
{
    for (const auto& wall : Walls)
    {
        wall.Render();
    }
}

void Game::Private::UpdateWalls()
{
    for (auto& wall : Walls)
    {
        wall.Update();
    }
    RemoveInactiveWalls();
}

void Game::Private::RemoveInactiveWalls()
{
    auto it = std::remove_if(Walls.begin(), Walls.end(), [](const Wall& wall) noexcept
        {
            return !wall.IsActive();
        });
    Walls.erase(it, Walls.end());
}

void Game::Private::RenderProjectiles() const noexcept
{
    for (const auto& projectile : Projectiles)
    {
        projectile.Render();
    }
}

void Game::Private::UpdateProjectiles()
{
    for (auto& projectile : Projectiles)
    {
        projectile.Update();
    }
    std::erase_if(Projectiles, [](const Projectile& p) { return !p.IsActive(); });
}

void Game::Private::DetectCollisions()
{
    for (auto& projectile : Projectiles)
    {
        if (projectile.GetType() == EntityType::PLAYER_PROJECTILE) { CheckPlayerCollision(projectile); }
        else if (projectile.GetType() == EntityType::ENEMY_PROJECTILE) { CheckEnemyCollision(projectile); }
        CheckWallCollisions(projectile);
    }
}

void Game::Private::CheckPlayerCollision(Projectile& projectile)
{
    for (auto& alien : Aliens)
    {
        if (alien.IsActive() && CheckCollision(alien.GetPosition(),
            alien.GetRadius(), projectile.GetLineStart(), projectile.GetLineEnd()))
        {
            projectile.SetActive(false);
            alien.SetActive(false);
            score += 100;
        }
    }
}

void Game::Private::CheckEnemyCollision(Projectile& projectile)
{
    if (CheckCollision({ player.GetXPosition(), Config::screenHeight - player.GetPlayerBaseHeight() },
        player.GetRadius(), projectile.GetLineStart(), projectile.GetLineEnd()))
    {
        projectile.SetActive(false);
        player.SetLives(player.GetLives() - 1);
        if (player.GetLives() <= 0) { End(); }
    }
}

void Game::Private::CheckWallCollisions(Projectile& projectile)
{
    for (auto& wall : Walls)
    {
        if (CheckCollision(wall.GetPosition(), wall.GetRadius(), projectile.GetLineStart(), projectile.GetLineEnd()))
        {
            projectile.SetActive(false);
            wall.SetHealth(wall.GetHealth() - 1);
            if (wall.GetHealth() <= 0) { wall.SetActive(false); }
        }
    }
}

bool Game::Private::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
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

void Game::Private::UpdateBackground()
{
    const auto playerBaseHeight = player.GetPlayerBaseHeight();
    const Vector2 playerPosition = { player.GetXPosition(), playerBaseHeight };
    const Vector2 screenCorner = { 0.0f, playerBaseHeight };
    const auto dx = screenCorner.x - playerPosition.x;
    const auto dy = screenCorner.y - playerPosition.y;
    const auto backgroundOffset = std::sqrt(dx * dx + dy * dy) * -1.0f;
    background.Update(backgroundOffset / Config::backgroundSpeed);
}

bool Game::Private::CheckNewHighScore() noexcept
{
    return true;
}