#include "Alien.hpp"
#include "Config.hpp"
#include <utility>
#include <print>

// Constructor
Alien::Alien()
    : position({ 0.0f, 0.0f }),
    radius(20.0f),
    speed(2.0f),
    active(true),
    moveRight(true),
    texture(Config::alienTexturePath)
{
    std::println("Alien created!");
}

// Move Constructor
Alien::Alien(Alien&& other) noexcept
    : position(std::move(other.position)),
    radius(std::move(other.radius)),
    speed(std::move(other.speed)),
    active(std::move(other.active)),
    moveRight(std::move(other.moveRight)),
    texture(std::move(other.texture))
{
    std::println("Alien moved!");
}

// Move Assignment
Alien& Alien::operator=(Alien&& other) noexcept
{
    if (this != &other)
    {
        position = std::move(other.position);
        radius = std::move(other.radius);
        speed = std::move(other.speed);
        active = std::move(other.active);
        moveRight = std::move(other.moveRight);
        texture = std::move(other.texture);

        std::println("Alien move-assigned!");
    }
    return *this;
}

// Update Method
void Alien::Update()
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

// Render Method
void Alien::Render() const noexcept
{
    DrawTexturePro(texture.GetTexture(), // Use the texture from TextureWrapper
        { 0, 0, (float)texture.GetTexture().width, (float)texture.GetTexture().height },
        { position.x, position.y, 100.0f, 100.0f },
        { 50.0f, 50.0f },
        0.0f,
        WHITE);
}

// Shoot Method
Projectile Alien::Shoot()
{
    Projectile newProjectile;
    newProjectile.SetPosition({ position.x, position.y + 40 });
    newProjectile.SetSpeed(5);
    newProjectile.SetType(EntityType::ENEMY_PROJECTILE);
    newProjectile.SetActive(true);
    return newProjectile;
}