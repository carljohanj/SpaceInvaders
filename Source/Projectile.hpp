#pragma once

#include "raylib.h"
#include "EntityType.hpp"

struct Projectile 
{
    Vector2 position = { 0, 0 };
    int speed = 15;
    bool active = true;
    EntityType type = {};

    Vector2 lineStart = { 0, 0 };
    Vector2 lineEnd = { 0, 0 };

    void Update();
    void Render(Texture2D texture) const noexcept;
};