#include "Gun.hpp"

Gun::Gun(float projectileSpeed, ProjectileType projectileType, Vector2 offset)
    : projectileSpeed(projectileSpeed), projectileType(projectileType), offset(offset) 
{}

Projectile Gun::Shoot(Vector2 origin) const noexcept 
{
    Projectile projectile;
    projectile.SetPosition({ origin.x + offset.x, origin.y + offset.y });
    projectile.SetSpeed(projectileSpeed);
    projectile.SetType(projectileType);
    projectile.SetActive(true);
    return projectile;
}