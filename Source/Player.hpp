#pragma once
#include "raylib.h"
#include "Projectile.hpp"

class Player
{
public:
    Player();
    ~Player();

    void Initialize();
    void Update();
    void Render();
    Projectile Shoot();
    float GetXPosition() const { return x_pos; }
    float GetPlayerBaseHeight() const { return player_base_height; }
    float GetRadius() const { return radius; }
    int GetLives() const { return lives; }
    int GetActiveTexture() const { return activeTexture; }
    void SetLives(int newLives) { lives = newLives; }
    void SetXPosition(float newXPos) { x_pos = newXPos; }

private:
    float x_pos = 0;
    float speed = 7;
    float player_base_height = 70.0f;
    float radius = 50;
    int lives = 3;
    int activeTexture = 0;

    Texture2D texture;  // Player-specific texture
};