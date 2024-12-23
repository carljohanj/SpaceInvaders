#include "Config.hpp"
#include "TextureLoadingException.hpp"
#include "Wall.hpp"
#include <iostream>
#include <utility>

Texture2D Wall::texture = { 0 };
int Wall::instanceCount = 0;

Wall::Wall()
{
    if (instanceCount == 0)
    {
        texture = LoadTexture(Config::wallTexturePath.data());
        if (texture.id == 0)
        {
            throw TextureLoadingException("Failed to load Wall texture!");
        }
        std::cout << "Wall texture loaded successfully." << std::endl;
    }
    instanceCount++;
}

Wall::~Wall()
{
    instanceCount--;
    std::cout << "Wall instance destroyed. Count: " << instanceCount << ", ID: " << this << std::endl;
    std::cout << "Current instance count: " << instanceCount << std::endl;

    if (instanceCount == 0 && texture.id != 0) {
        UnloadTexture(texture);
        texture = { 0 };
        std::cout << "Wall texture unloaded." << std::endl;
    }
}

Wall::Wall(Wall&& other) noexcept
    : position(std::exchange(other.position, { 0, 0 })),
    active(std::exchange(other.active, false)),
    health(std::exchange(other.health, 50)),
    radius(std::exchange(other.radius, 60))
{
    instanceCount++;
    std::cout << "Wall moved! Instance count: " << instanceCount << std::endl;
}

Wall& Wall::operator=(Wall&& other) noexcept
{
    if (this != &other)
    {
        position = std::exchange(other.position, { 0, 0 });
        active = std::exchange(other.active, false);
        health = std::exchange(other.health, 50);
        radius = std::exchange(other.radius, 60);
        std::cout << "Wall move-assigned! Instance count: " << instanceCount << std::endl;
    }
    return *this;
}

void Wall::Update()
{
    if (health <= 0) { active = false; }
}

void Wall::Render() const noexcept
{
    if (texture.id == 0)
    {
        std::cerr << "Wall texture is not valid!" << std::endl;
        return;
    }

    // Render the wall with texture
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, 200.0f, 200.0f },
        { 100.0f, 100.0f },
        0.0f,
        WHITE);

    // Render health
    DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}