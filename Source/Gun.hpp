#pragma once
#include "Projectile.hpp"
#include "EntityType.hpp"
#include "raylib.h"

class Gun
{
public:
    Gun(float speed, ProjectileType projectile, Vector2 offset);
    Projectile Shoot(Vector2 whoIsShooting) const noexcept;

private:
    float projectileSpeed;
    ProjectileType projectileType;
    Vector2 offset;
};