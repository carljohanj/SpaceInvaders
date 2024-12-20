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
    // Increment instance count when a new Alien is created
    std::cout << "Alien instance created. Count: " << instanceCount << std::endl;
}

Alien::~Alien() {
    if (instanceCount > 0) {
        // Decrement instance count when an Alien is destroyed
        std::cout << "Alien instance destroyed. Count: " << instanceCount << std::endl;

        // If no more aliens exist, unload the texture
        if (instanceCount == 0 && texture.id != 0) {
            UnloadTexture(texture);
            texture = { 0 }; // Reset texture to prevent accidental reuse
            std::cout << "Alien texture unloaded." << std::endl;
        }
    }
    else {
        std::cerr << "Error: Attempted to destroy an Alien instance when none exist!" << std::endl;
    }
}

void Alien::Update() {
    // Update position based on movement direction
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

void Alien::Render() {
    if (texture.id == 0) {
        std::cerr << "Alien texture is not valid!" << std::endl;
        return;
    }

    // Draw the texture with the proper source and destination rectangles
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { position.x, position.y, 100.0f, 100.0f }, // position and scale
        { 50.0f, 50.0f }, // origin (center of the texture)
        0.0f, // rotation (no rotation)
        WHITE);
}

Projectile Alien::Shoot() {
    Projectile newProjectile;
    newProjectile.position = { position.x, position.y + 40 };
    newProjectile.speed = 5;
    newProjectile.type = EntityType::ENEMY_PROJECTILE;
    newProjectile.active = true;
    return newProjectile;
}

void Alien::IncrementInstanceCount() {
    instanceCount++;
}

void Alien::DecrementInstanceCount() {
    instanceCount--;
}

int Alien::GetInstanceCount() {
    return instanceCount;
}