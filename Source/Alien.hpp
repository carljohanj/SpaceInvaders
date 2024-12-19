#pragma once
#include "raylib.h"
#include "Projectile.hpp"
#include "EntityType.hpp"

class Alien {
public:
    Alien();

    void Update();
    void Render(Texture2D texture);

    // Public members
    Vector2 position = { 0, 0 };
    float radius = 30;
    bool active = true;
    bool moveRight = true;
    int speed = 2;

    EntityType type = EntityType::ENEMY;
    Projectile Shoot();
};