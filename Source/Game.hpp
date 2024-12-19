#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "Resources.h"
#include "Player.hpp"
#include "Alien.hpp"
#include "Projectile.hpp"
#include "EntityType.hpp"

enum class State {
    STARTSCREEN,
    GAMEPLAY,
    ENDSCREEN
};

struct PlayerData {
    std::string name;
    int score;
};

struct Wall {
    Vector2 position;
    Rectangle rec;
    bool active = true;
    Color color;
    int health = 50;
    int radius = 60;

    void Render(Texture2D texture);
    void Update();
};

struct Star {
    Vector2 initPosition = { 0, 0 };
    Vector2 position = { 0, 0 };
    Color color = GRAY;
    float size = 0;
    void Update(float starOffset);
    void Render() const;
};

struct Background {
    std::vector<Star> Stars;

    void Initialize(int starAmount);
    void Update(float offset);
    void Render();
};

struct Game {
    // Gamestate
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

    // Game entities
    Player player;
    std::vector<Alien> Aliens;
    std::vector<Projectile> Projectiles;
    std::vector<Wall> Walls;
    Background background;

    // Resources
    Resources resources; // Resources included for asset management

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

    // Methods
    void Start();
    void End();
    void Continue();
    void Launch();
    void Update();
    void Render();
    void SpawnAliens();
    bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd);
    bool CheckNewHighScore();
    void InsertNewHighScore(std::string name);
};