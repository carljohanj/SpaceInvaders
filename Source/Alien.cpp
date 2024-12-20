#include "Alien.hpp"
#include <iostream>

// Define static members
Texture2D Alien::texture = { 0 };  // Initialize with an empty texture
int Alien::instanceCount = 0;       // Initialize instance count to zero

Alien::Alien() {
    // Load texture only once when the first instance is created
    if (instanceCount == 0) {
        texture = LoadTexture("./Assets/Alien.png");
        if (texture.id == 0) {
            std::cerr << "Failed to load Alien texture!" << std::endl;
        }
        else {
            std::cout << "Alien texture loaded successfully: ID " << texture.id << std::endl;
        }
    }
    instanceCount++;
    std::cout << "Alien instance created. Count: " << instanceCount << ", ID: " << this << std::endl; // Print unique ID (pointer)
}

Alien::~Alien() 
{
    std::cout << "Alien instance destroyed. Count: " << instanceCount << ", ID: " << this << std::endl; // Print unique ID (pointer)
    if (instanceCount == 1 && texture.id != 0) {
        UnloadTexture(texture);
        texture = { 0 };
        std::cout << "Alien texture unloaded." << std::endl;
    }
}

void Alien::Update() 
{
    if (moveRight) {
        position.x += speed; // Move right
        if (position.x >= GetScreenWidth() - radius) {
            moveRight = false;
            position.y += 50; // Move down
        }
    }
    else {
        position.x -= speed; // Move left
        if (position.x <= radius) {
            moveRight = true;
            position.y += 50; // Move down
        }
    }

    // Mark alien as inactive if it moves off-screen
    if (position.y > GetScreenHeight()) {
        active = false;
    }
}

void Alien::Render() 
{
    if (texture.id == 0) {
        std::cerr << "Alien texture is not valid!" << std::endl;
        return;
    }

    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, 100.0f, 100.0f }, // position and scale
        { 50.0f, 50.0f }, // origin (center of the texture)
        0.0f, // rotation (no rotation)
        WHITE);
}

Projectile Alien::Shoot() 
{
    Projectile newProjectile;
    newProjectile.position = { position.x, position.y + 40 };
    newProjectile.speed = 5;
    newProjectile.type = EntityType::ENEMY_PROJECTILE;
    newProjectile.active = true;
    return newProjectile;
}

void Alien::IncrementInstanceCount() { instanceCount++; }
void Alien::DecrementInstanceCount() { instanceCount--; }
int  Alien::GetInstanceCount()  { return instanceCount; }