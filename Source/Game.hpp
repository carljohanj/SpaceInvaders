#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Alien.hpp"
#include "Projectile.hpp"
#include "Wall.hpp"
#include "Star.hpp"
#include "Background.hpp"
#include "EntityType.hpp"
#include "GameWindow.hpp"
#include "Config.hpp"

enum class State
{
    STARTSCREEN,
    GAMEPLAY,
    ENDSCREEN
};

struct PlayerData
{
    std::string name;
    int score;
};

class Game
{
public:
    Game();
    void Run();

private:
    State gameState = State::STARTSCREEN;
    int score = 0;
    float shootTimer = 0;
    bool newHighScore = false;

    GameWindow window;
    Player player;
    Background background;
    std::vector<Alien> Aliens;
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;

    // Leaderboard (place in own struct?)
    std::string_view name;
    int letterCount = 0;
    Rectangle textBox = { Config::textBoxX, Config::textBoxY, Config::textBoxWidth, Config::textBoxHeight };
    bool mouseOnText = false;
    std::vector<PlayerData> Leaderboard = { {"Player 1", Config::initialHighScores[0]},
                                            {"Player 2", Config::initialHighScores[1]},
                                            {"Player 3", Config::initialHighScores[2]},
                                            {"Player 4", Config::initialHighScores[3]},
                                            {"Player 5", Config::initialHighScores[4]} };

    // UI Elements
    Vector2 playerPos;
    Vector2 cornerPos;
    float offset;

    void Start();
    void End() noexcept;
    void Continue() noexcept;
    void ResetGameState() noexcept;
    void Update();
    void UpdateAliens();
    void UpdateBackground();
    void UpdatePlayerInput();
    void UpdateProjectiles();
    void UpdateWalls();
    void Render() noexcept;
    void RenderAliens() noexcept;
    void RenderGameplay() noexcept;
    void RenderHUD() const noexcept;
    void RenderProjectiles() const noexcept;
    void RenderStartScreen() const noexcept;
    void RenderGameOverScreen() const noexcept;
    void RenderWalls() noexcept;
    void ResetAliens();
    void ResetWalls();
    void TriggerAlienShot();
    void RemoveInactiveAliens();
    void RemoveInactiveWalls();
    void DetectCollisions();
    void CheckPlayerCollision(Projectile& projectile);
    void CheckEnemyCollision(Projectile& projectile);
    void CheckWallCollisions(Projectile& projectile);
    bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd);
    bool CheckNewHighScore() noexcept;
    void InsertNewHighScore(std::string name);

    //ToDo: Hide all of the above private members and functions in this struct:
    struct Impl;
    alignas(std::max_align_t) std::byte implBuffer[128];
    Impl* pImpl;
};