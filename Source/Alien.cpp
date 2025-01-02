#include "Alien.hpp"
#include "Config.hpp"
#include <utility>
#include <print>

Alien::Alien()
    : position({ 0.0f, 0.0f }),
      radius(20.0f),  
      speed(2.0f),
      active(true),
      moveRight(true),
      gun(5.0f, ProjectileType::ENEMY_PROJECTILE, { 0, 40 }),
      texture(Config::alienTexturePath)
{
    std::println("Alien created!");
}

Alien::Alien(Alien&& other) noexcept
    : position(std::move(other.position)),
      radius(other.radius),
      speed(other.speed),
      active(other.active),
      moveRight(other.moveRight),
      gun(5.0f, ProjectileType::ENEMY_PROJECTILE, { 0, 40 }),
      texture(std::move(other.texture))
{
    std::println("Alien moved!");
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

        std::println("Alien move-assigned!");
    }
    return *this;
}

void Alien::Update() noexcept
{
    if (moveRight)
    {
        position.x += speed; // Move right
        if (position.x >= GetScreenWidth() - radius)
        {
            moveRight = false;
            position.y += 50; // Move down
        }
    }
    else
    {
        position.x -= speed; // Move left
        if (position.x <= radius)
        {
            moveRight = true;
            position.y += 50; // Move down
        }
    }
    if (position.y > GetScreenHeight()) { active = false; }
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