#pragma once
#include "raylib.h"
#include "Projectile.hpp"
#include "EntityType.hpp"

class Alien {
public:
    Alien();
    ~Alien();

    void Update();
    void Render(); // No arguments

    Projectile Shoot();

    // Getters
    Vector2 GetPosition() const { return position; }
    float GetRadius() const { return radius; }
    bool IsActive() const { return active; }

    // Setters
    void SetPosition(Vector2 newPosition) { position = newPosition; }
    void SetActive(bool isActive) { active = isActive; }

    static int GetInstanceCount();

    // Methods to update the instance count
    static void IncrementInstanceCount();
    static void DecrementInstanceCount();

    void EnsureTextureLoaded();

private:
    Vector2 position = { 0, 0 };
    float radius = 30;
    bool active = true;
    bool moveRight = true;
    int speed = 2;

    EntityType type = EntityType::ENEMY;

    static Texture2D texture; // Shared texture
    static int instanceCount;
};