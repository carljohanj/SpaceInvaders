#include "Wall.hpp"
#include "Config.hpp"
#include "Utilities.hpp"
#include <utility>
#include <iostream>

[[gsl::suppress(f .6, justification: "We want the constructor to throw if the texture can't be loaded")]]
Wall::Wall()
    : texture(Config::wallTexturePath, Config::wallWidth, Config::wallHeight),
    position({ 0, 0 }),
    active(true),
    health(50),
    radius(60){
}

Wall::Wall(Wall&& other) noexcept
    : texture(std::move(other.texture)),
    position(std::exchange(other.position, { 0, 0 })),
    active(std::exchange(other.active, false)),
    health(std::exchange(other.health, 50)),
    radius(std::exchange(other.radius, 60.0f)){
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