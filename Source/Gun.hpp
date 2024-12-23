#pragma once
#include "Projectile.hpp"
#include "EntityType.hpp"
#include "raylib.h"

class Gun 
{
public:
    Gun(float projectileSpeed, EntityType projectileType, Vector2 offset);

    Projectile Shoot(Vector2 origin) const noexcept;

private:
    float projectileSpeed;
    EntityType projectileType;
    Vector2 offset;
};