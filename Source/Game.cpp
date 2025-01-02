#include "Config.hpp"
#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include "StackManager.hpp"
#include <random>
#include <ranges>

struct Game::Private
{
    State gameState = State::STARTSCREEN;
    int score = 0;
    float shootTimer = Config::defaultCooldown;
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
    void UpdateEverything();
    void UpdatePlayerInput();
    void CreateNewAlien(int alienRow);
    void RenderAliens() noexcept;
    void UpdateAliens();
    void UpdateAlienstates() noexcept;
    void TriggerAlienShot() noexcept;
    void UpdateTimeSinceLastShot() noexcept;
    bool CanFireShot() const noexcept;
    void FireShot() noexcept;
    size_t GetRandomAlien(size_t range) noexcept;
    void ResetShootTimer() noexcept;
    void RemoveInactiveAliens();
    void ResetWalls();
    void RenderWalls() noexcept;
    void UpdateWalls() noexcept;
    void RenderProjectiles() const noexcept;
    void UpdateProjectiles();
    void DetectCollisions() noexcept;
    void CheckPlayerCollision(Projectile& projectile) noexcept;
    void CheckEnemyCollision(Projectile& projectile) noexcept;
    void CheckWallCollisions(Projectile& projectile) noexcept;
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
        UpdateEverything();
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

void Game::Private::UpdateEverything()
{
    UpdatePlayerInput();
    UpdateAliens();
    UpdateProjectiles();
    DetectCollisions();
    UpdateWalls();
    UpdateBackground();
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
    if (!Aliens.empty()) return;

    for (const int row : std::views::iota(0, Config::alienFormationHeight))
    {
        CreateNewAlien(row);
    }
}

inline void Game::Private::CreateNewAlien(int alienRow)
{
    for (const int alienCol : std::views::iota(0, Config::alienFormationWidth))
    {
        Aliens.emplace_back().SetPosition({
        static_cast<float>(Config::alienFormationX + 450 + (alienCol * Config::alienSpacing)),
        static_cast<float>(Config::alienFormationY + (alienRow * Config::alienSpacing))
            });
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
    UpdateAlienstates();
    if (Aliens.empty()) { ResetAliens(); }
}

inline void Game::Private::UpdateAlienstates() noexcept
{
    for (auto& alien : Aliens)
    {
        alien.Update();
        if (alien.GetPosition().y > Config::screenHeight - Config::playerBaseHeight) { End(); }
    }
}

void Game::Private::TriggerAlienShot() noexcept
{
    UpdateTimeSinceLastShot();
    if (CanFireShot())
    {
        FireShot();
        ResetShootTimer();
    }
}

inline void Game::Private::UpdateTimeSinceLastShot() noexcept { shootTimer += GetFrameTime(); }

[[nodiscard]] inline bool Game::Private::CanFireShot() const noexcept
{
    return Aliens.empty() ? false : shootTimer >= Config::alienShootInterval;
}

inline void Game::Private::FireShot() noexcept
{
    auto const& randomAlien = Aliens[GetRandomAlien(Aliens.size())];
    if (randomAlien.IsActive()) { Projectiles.push_back(randomAlien.Shoot()); }
}

[[nodiscard]] inline size_t Game::Private::GetRandomAlien(size_t range) noexcept
{
    static std::mt19937 gen{ std::random_device{}() };
    return std::uniform_int_distribution<size_t>{0, range - 1}(gen);
}

inline void Game::Private::ResetShootTimer() noexcept { shootTimer = Config::defaultCooldown; }


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

void Game::Private::UpdateWalls() noexcept
{
    std::erase_if(Walls, [](const Wall& wall) noexcept { return !wall.IsActive(); });
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
    std::erase_if(Projectiles, [](const Projectile& p) noexcept { return !p.IsActive(); });
}

void Game::Private::DetectCollisions() noexcept
{
    for (auto& projectile : Projectiles)
    {
        if (!projectile.IsActive()) { continue; }
        switch (projectile.GetType())
        {
        case ProjectileType::PLAYER_PROJECTILE:
            CheckEnemyCollision(projectile);
            break;
        case ProjectileType::ENEMY_PROJECTILE:
            CheckPlayerCollision(projectile);
            break;
        }
        CheckWallCollisions(projectile);
    }
}

inline void Game::Private::CheckEnemyCollision(Projectile& projectile) noexcept
{
    for (auto& alien : Aliens)
    {
        if (!alien.IsActive()) { continue; }
        if (CheckCollisionCircles(alien.GetPosition(), alien.GetRadius(),
            { projectile.GetLineStart().x, projectile.GetLineStart().y }, 1.0f))
        {
            alien.SetActive(false);
            projectile.SetActive(false);
            score += 100;
            break;
        }
    }
}

inline void Game::Private::CheckWallCollisions(Projectile& projectile) noexcept
{
    for (auto& wall : Walls)
    {
        if (!wall.IsActive()) { continue; }
        if (CheckCollisionPointRec(projectile.GetPosition(), wall.GetRectangle()))
        {
            wall.SetHealth(wall.GetHealth() - 1);
            if (wall.GetHealth() <= 0) { wall.SetActive(false); }
            projectile.SetActive(false);
            break;
        }
    }
}

inline void Game::Private::CheckPlayerCollision(Projectile& projectile) noexcept
{
    if (CheckCollisionCircles(player.GetPosition(), player.GetRadius(),
        { projectile.GetLineStart().x, projectile.GetLineStart().y }, 1.0f))
    {
        player.SetLives(player.GetLives() - 1);
        projectile.SetActive(false);
        if (player.GetLives() <= 0) { End(); }
    }
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

bool Game::Private::CheckNewHighScore() noexcept { return true; }