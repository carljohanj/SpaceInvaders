#include "Projectile.hpp"
#include "TextureLoadingException.hpp"
#include <iostream>

Texture2D Projectile::texture = { 0 };
int Projectile::instanceCount = 0;

Projectile::Projectile()
    : position({ 0, 0 }), speed(15), active(true), lineStart({ 0, 0 }), lineEnd({ 0, 0 })
{
    if (instanceCount == 0)
    {
        texture = LoadTexture("./Assets/Laser.png");
        if (texture.id == 0)
        {
            throw std::runtime_error("Failed to load Projectile texture.");
        }
    }
    ++instanceCount;
}

Projectile::~Projectile()
{
    instanceCount--;
    if (instanceCount == 0 && texture.id != 0)
    {
        UnloadTexture(texture);
        texture = { 0 };
        std::cout << "Projectile texture unloaded." << std::endl;
    }
}

Projectile::Projectile(const Projectile& other)
    : position(other.position),
    speed(other.speed),
    active(other.active),
    type(other.type),
    lineStart(other.lineStart),
    lineEnd(other.lineEnd)
{
    instanceCount++;
    std::cout << "Projectile copied! Instance count: " << instanceCount << std::endl;
}

Projectile& Projectile::operator=(const Projectile& other)
{
    if (this != &other)
    {
        position = other.position;
        speed = other.speed;
        active = other.active;
        type = other.type;
        lineStart = other.lineStart;
        lineEnd = other.lineEnd;
        std::cout << "Projectile copy-assigned! Instance count remains: " << instanceCount << std::endl;
    }
    return *this;
}

Projectile::Projectile(Projectile&& other) noexcept
    : position(std::move(other.position)),
    speed(other.speed),
    active(other.active),
    type(other.type),
    lineStart(std::move(other.lineStart)),
    lineEnd(std::move(other.lineEnd))
{
    instanceCount++;
    std::cout << "Projectile moved! Instance count: " << instanceCount << std::endl;
}

Projectile& Projectile::operator=(Projectile&& other) noexcept
{
    if (this != &other)
    {
        position = std::move(other.position);
        speed = other.speed;
        active = other.active;
        type = other.type;
        lineStart = std::move(other.lineStart);
        lineEnd = std::move(other.lineEnd);
        other.active = false;
        std::cout << "Projectile move-assigned! Instance count remains: " << instanceCount << std::endl;
    }
    return *this;
}

void Projectile::Update()
{
    position.y += speed;
    lineStart.y = position.y - 15;
    lineEnd.y = position.y + 15;
    lineStart.x = position.x;
    lineEnd.x = position.x;

    if (position.y < 0 || position.y > GetScreenHeight())
    {
        active = false;
    }
}

void Projectile::Render() const noexcept
{
    if (texture.id == 0)
    {
        std::cerr << "Projectile texture is not valid!" << std::endl;
        return;
    }

    DrawTexturePro(texture,
        { 0, 0, 176, 176 },
        { position.x, position.y, 50, 50 },
        { 25, 25 },
        0,
        WHITE);
}