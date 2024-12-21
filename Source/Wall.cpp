#include "Wall.hpp"
#include "TextureLoadingException.hpp"
#include <iostream>

Texture2D Wall::texture = { 0 };  // Initialize static texture to 0
int Wall::instanceCount = 0;  // Initialize instance count to 0

Wall::Wall()
{
    // Only load the texture the first time a Wall instance is created
    if (instanceCount == 0)
    {
        texture = LoadTexture("./Assets/Barrier.png");
        if (texture.id == 0) {
            throw TextureLoadingException("Failed to load Wall texture: ./Assets/Barrier.png");
        }
        std::cout << "Wall texture loaded successfully." << std::endl;
    }
    instanceCount++;  // Increment instance count
}

Wall::~Wall()
{
    // Decrement instance count on destruction
    std::cout << "Wall instance destroyed. Count: " << instanceCount << ", ID: " << this << std::endl;
    std::cout << "Current instance count: " << Wall::GetInstanceCount() << std::endl;
    DecrementInstanceCount();

    // If this is the last instance, unload the texture
    if (instanceCount == 1 && texture.id != 0) {
        UnloadTexture(texture);  // Unload texture from GPU memory
        texture = { 0 };  // Reset the texture handle to avoid using an invalid texture
        std::cout << "Wall texture unloaded." << std::endl;
    }
}

void Wall::Update()
{
    if (health <= 0) {
        active = false;  // Mark wall as inactive if health is 0 or below
    }
}

void Wall::Render() const noexcept
{
    if (texture.id == 0) {
        std::cerr << "Wall texture is not valid!" << std::endl;
        return;  // Skip rendering if texture is invalid
    }

    // Render the wall with texture
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, 200.0f, 200.0f },  // Position and size
        { 100.0f, 100.0f },  // Origin (center of the texture)
        0.0f,  // No rotation
        WHITE);

    // Render health
    DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

void Wall::IncrementInstanceCount() { instanceCount++; }
void Wall::DecrementInstanceCount() { instanceCount--; }
int Wall::GetInstanceCount() { return instanceCount; }