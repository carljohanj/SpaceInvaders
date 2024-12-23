#include "Config.hpp"
#include "Projectile.hpp"
#include <utility>
#include <iostream>

Projectile::Projectile()
    : texture(Config::projectileTexturePath)
{
    position = { 0, 0 };
    speed = 15;
    active = true;
    lineStart = { 0, 0 };
    lineEnd = { 0, 0 };
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
    if (!active) return;

    DrawTexturePro(texture.GetTexture(),
        { 0, 0, 176, 176 },
        { position.x, position.y, 50, 50 },
        { 25, 25 },
        0,
        WHITE);
}

Projectile::Projectile(Projectile&& other) noexcept
    : position(std::move(other.position)),
    speed(std::exchange(other.speed, 0)),
    active(std::exchange(other.active, false)),
    type(std::move(other.type)),
    lineStart(std::move(other.lineStart)),
    lineEnd(std::move(other.lineEnd)),
    texture(std::move(other.texture))
{
    std::cout << "Projectile moved!" << std::endl;
}

Projectile& Projectile::operator=(Projectile&& other) noexcept
{
    if (this != &other)
    {
        position = std::move(other.position);
        speed = std::exchange(other.speed, 0);
        active = std::exchange(other.active, false);
        type = std::move(other.type);
        lineStart = std::move(other.lineStart);
        lineEnd = std::move(other.lineEnd);
        texture = std::move(other.texture);
        std::cout << "Projectile move-assigned!" << std::endl;
    }
    return *this;
}