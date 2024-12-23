#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "Resources.h"
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
    void Launch();

private:
    State gameState = State::STARTSCREEN;
    int score = 0;               // Current game score
    float shootTimer = 0;        // Timer for alien shooting
    bool newHighScore = false;   // Tracks if a new high score was achieved

    GameWindow window;           // The game window
    Resources resources;         // Game resources (textures, fonts, etc.)
    Player player;               // The player entity
    std::vector<Alien> Aliens;   // List of aliens in the game
    std::vector<Projectile> Projectiles; // List of projectiles in the game
    std::vector<Wall> Walls;     // List of walls in the game
    Background background;       // The game background

    // Leaderboard (place in own struct?)
    std::string_view name;
    int letterCount = 0;
    Rectangle textBox = { textBoxX, textBoxY, textBoxWidth, textBoxHeight };
    bool mouseOnText = false;
    std::vector<PlayerData> Leaderboard = { {"Player 1", initialHighScores[0]},
                                            {"Player 2", initialHighScores[1]},
                                            {"Player 3", initialHighScores[2]},
                                            {"Player 4", initialHighScores[3]},
                                            {"Player 5", initialHighScores[4]} };

    // UI Elements
    Vector2 playerPos;
    Vector2 cornerPos;
    float offset;

    void Start();
    void End() noexcept;
    void Continue() noexcept;
    void Update();
    void UpdateAliens();
    void UpdateBackground();
    void UpdatePlayerInput();
    void UpdateProjectiles();
    void UpdateWalls();
    void Render();
    void RenderAliens() noexcept;
    void RenderGameplay();
    void RenderHUD() const noexcept;
    void RenderProjectiles() const noexcept;
    void RenderStartScreen() const noexcept;
    void RenderWalls() noexcept;
    void DrawAliens();
    void DrawWalls();
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