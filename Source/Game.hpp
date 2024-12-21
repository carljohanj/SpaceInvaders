#pragma once
#include "raylib.h"
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
    State gameState = State::STARTSCREEN;

    // Score
    int score;

    // Walls
    int wallCount = 5;

    // Alien formation
    int formationWidth = 8;
    int formationHeight = 5;
    int alienSpacing = 80;
    int formationX = 100;
    int formationY = 50;

    // Shooting
    float shootTimer = 0;

    // High score tracking
    bool newHighScore = false;

    GameWindow window;
    Resources resources;
    Player player;
    std::vector<Alien> Aliens;
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;
    Background background;

    // Leaderboard
    std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

    // UI
    Vector2 playerPos;
    Vector2 cornerPos;
    float offset;

    char name[10] = "\0";
    int letterCount = 0;

    Rectangle textBox = { 600, 500, 225, 50 };
    bool mouseOnText = false;

    Game();

    // Methods
    void Start();
    void End();
    void Run();
    void Continue();
    void Launch();
    void Update();
    void Render();
    void SpawnAliens();
    void RemoveInactiveAliens();
    void RemoveInactiveWalls();
    bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd);
    bool CheckNewHighScore();
    void InsertNewHighScore(std::string name);
};