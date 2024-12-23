#include "Alien.hpp"
#include "Config.hpp"
#include "TextureLoadingException.hpp"
#include <iostream>
#include <utility>
#include <print>

Texture2D Alien::texture = { 0 };
int Alien::instanceCount = 0;

Alien::Alien()
    : position({ 0.0f, 0.0f }), radius(20.0f), speed(2.0f), active(true), moveRight(true)
{
    if (instanceCount == 0)
    {
        texture = LoadTexture(Config::alienTexturePath.data());
        if (texture.id == 0)
        {
            throw TextureLoadingException("Failed to load Alien texture!");
        }
    }
    instanceCount++;
    std::println("Alien born! Wohoo! Instance count: {}", instanceCount);
}

// Copy Constructor
Alien::Alien(const Alien& other)
    : position(other.position),
    radius(other.radius),
    speed(other.speed),
    active(other.active),
    moveRight(other.moveRight)
{
    instanceCount++;
    std::println("Alien copied! Instance count: {}", instanceCount);
}

// Move Constructor
Alien::Alien(Alien&& other) noexcept
    : position(std::move(other.position)),
    radius(std::move(other.radius)),
    speed(std::move(other.speed)),
    active(std::move(other.active)),
    moveRight(std::move(other.moveRight))
{
    instanceCount++;
    std::println("Alien moved! Instance count remains: {}", instanceCount);
}

// Destructor
Alien::~Alien()
{
    instanceCount--;
    std::println("Alien instance destroyed! Oh no! Instance count: {}", instanceCount);

    if (instanceCount == 0 && texture.id != 0)
    {
        UnloadTexture(texture);
        texture = { 0 };
        std::println("Alien texture unloaded");
    }
}

Alien& Alien::operator=(const Alien& other)
{
    if (this != &other)
    {
        position = other.position;
        radius = other.radius;
        speed = other.speed;
        active = other.active;
        moveRight = other.moveRight;

        std::println("Alien assigned via copy!");
    }
    return *this;
}

Alien& Alien::operator=(Alien&& other) noexcept
{
    if (this != &other)
    {
        std::swap(position, other.position);
        std::swap(radius, other.radius);
        std::swap(speed, other.speed);
        std::swap(active, other.active);
        std::swap(moveRight, other.moveRight);

        std::println("Alien assigned via move using std::swap!");
    }
    return *this;
}

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

void Alien::Render() const noexcept
{
    if (texture.id == 0)
    {
        std::cerr << "Alien texture is not valid!" << std::endl;
        return;
    }

    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, 100.0f, 100.0f },
        { 50.0f, 50.0f },
        0.0f,
        WHITE);
}

Projectile Alien::Shoot()
{
    Projectile newProjectile;
    newProjectile.SetPosition({ position.x, position.y + 40 });
    newProjectile.SetSpeed(5);
    newProjectile.SetType(EntityType::ENEMY_PROJECTILE);
    newProjectile.SetActive(true);
    return newProjectile;
}