#include "Config.hpp"
#include "Projectile.hpp"
#include "Utilities.hpp"
#include <utility>
#include <gsl/gsl>

Projectile::Projectile(Vector2 position, float speed, ProjectileType type, const Texture2D& sharedTexture) noexcept
    : position(position), speed(static_cast<int>(speed)), type(type), texture(&sharedTexture), active(true)
{
    lineStart = { 0, 0 };
    lineEnd = { 0, 0 };
}

Projectile::~Projectile() = default;

Projectile::Projectile(Projectile&& other) noexcept
    : position(std::move(other.position)),
    speed(std::exchange(other.speed, 0)),
    active(std::exchange(other.active, false)),
    type(std::move(other.type)),
    lineStart(std::move(other.lineStart)),
    lineEnd(std::move(other.lineEnd)),
    texture(other.texture)
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

Vector2 Projectile::GetPositionForCollision() const noexcept { return { position.x, position.y }; }

Vector2 Projectile::GetPosition() const noexcept { return position; }

bool Projectile::IsActive() const noexcept { return active; }

int Projectile::GetSpeed() const noexcept { return speed; }

ProjectileType Projectile::GetType() const noexcept { return type; }

Vector2 Projectile::GetLineStart() const noexcept { return lineStart;}

Vector2 Projectile::GetLineEnd() const noexcept { return lineEnd; }

void Projectile::SetPosition(Vector2 newPosition) noexcept { position = newPosition; }

void Projectile::SetSpeed(int newSpeed) noexcept { speed = newSpeed; }

void Projectile::SetType(ProjectileType newType) noexcept { type = newType; }

void Projectile::SetActive(bool isActive) noexcept { active = isActive; }