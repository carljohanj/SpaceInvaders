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
    float GetXPosition() const noexcept;
    float GetPlayerBaseHeight() const noexcept;
    float GetRadius() const noexcept;
    int GetLives() const noexcept;
    void SetLives(int newLives) noexcept;
    void SetXPosition(float newXPos) noexcept;

private:
    float x_pos;
    float speed;
    float player_base_height;
    float radius;
    int lives;
    Gun gun;
    TextureWrapper texture;
};