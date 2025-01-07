#pragma once
#include "raylib.h"
#include "Gun.hpp"
#include "Projectile.hpp"
#include "TextureWrapper.hpp"

class Player
{
public:
    Player();

    void Reset() noexcept;
    void Update() noexcept;
    void Render() const;
    Projectile Shoot() const noexcept;
    Vector2 GetPosition() const noexcept;
    float GetXPosition() const noexcept { return x_pos; }
    float GetPlayerBaseHeight() const noexcept { return player_base_height; }
    float GetRadius() const noexcept { return radius; }
    int GetLives() const noexcept { return lives; }
    void SetLives(int newLives) noexcept { lives = newLives; }
    void SetXPosition(float newXPos) noexcept { x_pos = newXPos; }

private:
    float x_pos = 0;
    float speed = 7;
    float player_base_height = 70.0f;
    float radius = 50;
    int lives = 3;
    Gun gun;
    TextureWrapper texture;
};