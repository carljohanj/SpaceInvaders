#include "Config.hpp"
#include "Projectile.hpp"
#include <utility>
#include <iostream>

Projectile::Projectile(Vector2 position, float speed, ProjectileType type, const Texture2D& sharedTexture)
    : position(position), speed(static_cast<int>(speed)), type(type), texture(&sharedTexture), active(true)
{
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

    DrawTexturePro(*texture, // Use shared texture
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
    texture(other.texture) // Copy shared reference
{
    other.texture = nullptr;
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
        texture = other.texture;
        other.texture = nullptr;
    }
    return *this;
}