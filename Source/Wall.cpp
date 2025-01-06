#include "Wall.hpp"
#include "Config.hpp"
#include <print>
#include <utility>
#include <iostream>

Wall::Wall()
    : texture(Config::wallTexturePath) {
}

Wall::Wall(Wall&& other) noexcept
    : position(std::exchange(other.position, { 0, 0 })),
      active(std::exchange(other.active, false)),
      health(std::exchange(other.health, 50)),
      radius(std::exchange(other.radius, 60)),
      texture(std::move(other.texture)) {
}

Wall& Wall::operator=(Wall&& other) noexcept
{
    if (this != &other)
    {
        position = std::exchange(other.position, { 0, 0 });
        active = std::exchange(other.active, false);
        health = std::exchange(other.health, 50);
        radius = std::exchange(other.radius, 60);
        texture = std::move(other.texture);
    }
    return *this;
}

void Wall::Render() const noexcept
{
    RenderWallText();
    RenderHealth();
}

inline void Wall::RenderWallText() const noexcept
{
    const auto& textureRef = texture.GetTexture();
    DrawTexturePro(textureRef,
        { 0, 0, (float)textureRef.width, (float)textureRef.height },
        { position.x, position.y, 200.0f, 200.0f },
        { 100.0f, 100.0f },
        0.0f,
        WHITE);

}

inline void Wall::RenderHealth() const noexcept
{
    DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

Rectangle Wall::GetRectangle() const noexcept
{
    return { position.x - radius, position.y - radius, radius * 2, radius * 2 };
}