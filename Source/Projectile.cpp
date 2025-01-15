#include "Config.hpp"
#include "Projectile.hpp"
#include "Utilities.hpp"
#include <utility>

[[gsl::suppress(f .6, justification: "We want the constructor to throw if the texture can't be loaded")]]
Projectile::Projectile(Vector2 position, float speed, ProjectileType type, const Texture2D& sharedTexture)
    : position(position), speed(static_cast<int>(speed)), type(type), texture(&sharedTexture), active(true)
{
    lineStart = { 0, 0 };
    lineEnd = { 0, 0 };
}

void Projectile::Update() noexcept
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
    Util::RenderRectangle(*texture, position, Config::projectileWidth, Config::projectileHeight);
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