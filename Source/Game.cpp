#include "Config.hpp"
#include "Game.hpp"
#include <algorithm>
#include <cassert>
#include "StackManager.hpp"
#include <random>
#include <ranges>

using alien = size_t;
inline constexpr float shootTimerReset = 0.0f;

struct Game::Private
{
    State gameState = State::STARTSCREEN;
    int score = 0;
    float shootTimer = Config::defaultCooldown;
    bool newHighScore = false;

    GameWindow window;
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
    alien GetRandomAlien(alien range);
    void ResetShootTimer() noexcept;
    void RemoveInactiveAliens() noexcept;
    void ResetWalls();
    void RenderWalls() noexcept;
    void UpdateWalls() noexcept;
    void RenderProjectiles() const noexcept;
    void UpdateProjectiles();
    void UpdateBackground() noexcept;
    void DetectCollisions() noexcept;
    void CheckPlayerCollision(Projectile& projectile) noexcept;
    void CheckEnemyCollision(Projectile& projectile) noexcept;
    void CheckWallCollisions(Projectile& projectile) noexcept;
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
    window.ResetBackground();
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
        window.RenderStartScreen();
        break;
    case State::GAMEPLAY:
        RenderGameplay();
        break;
    case State::ENDSCREEN:
        window.RenderGameOverScreen();
        break;
    }
}

void Game::Private::End() noexcept
{
    Projectiles.clear();
    Walls.clear();
    Aliens.clear();
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
    window.RenderBackground();
    window.RenderHUD(score, player.GetLives());
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

[[nodiscard]] inline alien Game::Private::GetRandomAlien(alien range)
{
    static std::mt19937 gen{ std::random_device{}() };
    return std::uniform_int_distribution<size_t>{0, range - 1}(gen);
}

inline void Game::Private::ResetShootTimer() noexcept { shootTimer = shootTimerReset; }


inline void Game::Private::RemoveInactiveAliens() noexcept
{
    std::erase_if(Aliens, [](const Alien& alien) noexcept { return !alien.IsActive(); });
}

void Game::Private::ResetWalls()
{
    Walls.clear();
    constexpr auto wallDistance = Config::screenWidth / (Config::defaultWallCount + 1);
    for (const int i : std::views::iota(1, Config::defaultWallCount + 1))
    {
        Walls.emplace_back().SetPosition({ wallDistance * static_cast<float>(i),
                                           Config::screenHeight - Config::wallOffset });
    }
}

void Game::Private::RenderWalls() noexcept
{
    for (auto& wall : Walls)        //ToDo: See if we can make this const
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

void Game::Private::UpdateBackground() noexcept
{
    window.UpdateBackground(player.GetXPosition());
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

//ToDo: If we hide these in some kind of collision manager we can get some ugly code out of the way
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