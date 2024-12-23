#include "Wall.hpp"
#include "Config.hpp"
#include <print>
#include <utility>
#include <iostream>

Wall::Wall()
    : texture(Config::wallTexturePath)
{
    std::println("Wall created!");
}

Wall::~Wall()
{
    std::println("Wall instance destroyed.");
}

Wall::Wall(Wall&& other) noexcept
    : position(std::exchange(other.position, { 0, 0 })),
      active(std::exchange(other.active, false)),
      health(std::exchange(other.health, 50)),
      radius(std::exchange(other.radius, 60)),
      texture(std::move(other.texture))
{
    std::println("Wall moved!");;
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
        std::println("Wall move-assigned!");
    }
    return *this;
}

void Wall::Update() noexcept
{
    if (health <= 0) { active = false; }
}

void Wall::Render() const noexcept
{
    const auto& textureRef = texture.GetTexture();
    if (textureRef.id == 0)
    {
        std::cerr << "Wall texture is not valid!" << std::endl;
        return;
    }

    // Render the wall with texture
    DrawTexturePro(textureRef,
        { 0, 0, (float)textureRef.width, (float)textureRef.height },
        { position.x, position.y, 200.0f, 200.0f },
        { 100.0f, 100.0f },
        0.0f,
        WHITE);

    // Render health
    DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}