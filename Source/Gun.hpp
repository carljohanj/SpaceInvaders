#pragma once
#include "Projectile.hpp"
#include "EntityType.hpp"
#include "TextureWrapper.hpp"
#include "Config.hpp"
#include "raylib.h"

class Gun
{
public:
    Gun(float speed, ProjectileType projectileType, Vector2 offset) noexcept;
    Projectile Shoot(Vector2 whoIsShooting) const noexcept;

private:
    float projectileSpeed;
    ProjectileType projectileType;
    Vector2 offset;
    TextureWrapper projectileTexture;
};