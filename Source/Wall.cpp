#include "Wall.hpp"
#include "Config.hpp"
#include "Utilities.hpp"
#include <utility>
#include <iostream>

Wall::Wall()
    : texture(Config::wallTexturePath, Config::wallWidth, Config::wallHeight),
    position({ 0, 0 }),
    active(true),
    health(50),
    radius(60.0f) {
}

Wall::Wall(Wall&& other) noexcept
    : texture(std::move(other.texture)),
    position(std::exchange(other.position, { 0, 0 })),
    active(std::exchange(other.active, false)),
    health(std::exchange(other.health, 50)),
    radius(std::exchange(other.radius, 60.0f))
{
}

Wall& Wall::operator=(Wall&& other) noexcept
{
    if (this != &other)
    {
        texture = std::move(other.texture);
        position = std::exchange(other.position, { 0, 0 });
        active = std::exchange(other.active, false);
        health = std::exchange(other.health, 50);
        radius = std::exchange(other.radius, 60.0f);
    }
    return *this;
}

void Wall::Render() const
{
    Util::RenderRectangle(texture.GetTexture(), position, Config::wallWidth, Config::wallHeight);
    RenderHealth();
}

void Wall::RenderHealth() const noexcept
{
    DrawText(TextFormat("%i", health),
        static_cast<int>(position.x) - 21,
        static_cast<int>(position.y) + 10,
        40,
        RED);
}

Rectangle Wall::GetRectangle() const noexcept
{
    return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}

Vector2 Wall::GetPosition() const noexcept { return position; }

void Wall::SetPosition(Vector2 newPosition) noexcept { position = newPosition; }

bool Wall::IsActive() const noexcept { return active; }

void Wall::SetActive(bool isActive) noexcept { active = isActive; }

int Wall::GetHealth() const noexcept { return health; }

void Wall::SetHealth(int newHealth) noexcept { health = newHealth; }

float Wall::GetRadius() const noexcept { return radius; }