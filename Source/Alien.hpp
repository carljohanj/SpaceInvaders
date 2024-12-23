#pragma once
#include "raylib.h"
#include "Projectile.hpp"
#include "EntityType.hpp"

class Alien 
{
public:
    Alien();
    ~Alien();
    void Update();
    void Render() const noexcept;
    Projectile Shoot();
    static int GetInstanceCount() noexcept;
    static void IncrementInstanceCount() noexcept;
    static void DecrementInstanceCount() noexcept;
    
    Vector2 GetPosition() const noexcept { return position; }
    float GetRadius() const noexcept { return radius; }
    bool IsActive() const noexcept { return active; }
    void SetPosition(Vector2 newPosition) noexcept { position = newPosition; }
    void SetActive(bool isActive) noexcept { active = isActive; }

private:
    Vector2 position = { 0, 0 };
    float radius = 30;
    bool active = true;
    bool moveRight = true;
    int speed = 2;
    EntityType type = EntityType::ENEMY;

    static Texture2D texture;
    static int instanceCount;
};