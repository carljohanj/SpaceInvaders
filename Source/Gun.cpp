#include "Gun.hpp"
#include "Config.hpp"

Gun::Gun(float speed, ProjectileType projectile, Vector2 offset) noexcept
    : projectileSpeed(speed), projectileType(projectile), offset(offset) {
}

Projectile Gun::Shoot(Vector2 whoIsShooting) const noexcept 
{
    Projectile projectile;
    projectile.SetPosition({ whoIsShooting.x + offset.x, whoIsShooting.y + offset.y });
    projectile.SetSpeed(projectileSpeed);
    projectile.SetType(projectileType);
    projectile.SetActive(true);
    return projectile;
}