#include "Alien.hpp"
#include "Config.hpp"
#include <utility>

Alien::Alien()
    : position({ 0.0f, 0.0f }),
      radius(40.0f),  
      speed(2.0f),
      active(true),
      moveRight(true),
      gun(Config::alienProjectileSpeed, ProjectileType::ENEMY_PROJECTILE, { 0, 40 }),
      texture(Config::alienTexturePath) {
}

Alien::Alien(Alien&& other) noexcept
    : position(std::move(other.position)),
      radius(other.radius),
      speed(other.speed),
      active(other.active),
      moveRight(other.moveRight),
      gun(Config::alienProjectileSpeed, ProjectileType::ENEMY_PROJECTILE, { 0, 40 }),
      texture(std::move(other.texture)) {
}

Alien& Alien::operator=(Alien&& other) noexcept
{
    if (this != &other)
    {
        position = std::move(other.position);
        radius = other.radius;
        speed = other.speed;
        active = other.active;
        moveRight = other.moveRight;
        texture = std::move(other.texture);
    }
    return *this;
}

void Alien::Update() noexcept
{
    if (moveRight)
    {
        MoveToRight();
        if (position.x >= GetScreenWidth() - radius) { MoveDown(); }
    }
    else
    {
        MoveToLeft();
        if (position.x <= radius) { MoveDown(); }
    }
    if (position.y > GetScreenHeight()) { active = false; }
}

inline void Alien::MoveToRight() noexcept { position.x += speed; }

inline void Alien::MoveToLeft() noexcept { position.x -= speed; }

inline void Alien::MoveDown() noexcept
{
    moveRight = !moveRight;
    position.y += 50;
}

void Alien::Render() const noexcept
{
    DrawTexturePro(texture.GetTexture(),
        { 0, 0, (float)texture.GetTexture().width, (float)texture.GetTexture().height },
        { position.x, position.y, 100.0f, 100.0f },
        { 50.0f, 50.0f },
        0.0f,
        WHITE);
}

Projectile Alien::Shoot() const noexcept { return gun.Shoot(position); }