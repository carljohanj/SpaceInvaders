#include "Gun.hpp"

Gun::Gun(float speed, ProjectileType projectileType, Vector2 offset) noexcept
    : projectileSpeed(speed),
    projectileType(projectileType),
    offset(offset),
    projectileTexture(Config::projectileTexturePath)
{
}

Projectile Gun::Shoot(Vector2 whoIsShooting) const noexcept
{
    Projectile projectile(
        { whoIsShooting.x + offset.x, whoIsShooting.y + offset.y },
        projectileSpeed,
        projectileType,
        projectileTexture.GetTexture()
    );

    projectile.SetActive(true);
    return projectile;
}