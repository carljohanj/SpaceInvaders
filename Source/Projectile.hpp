#pragma once

#include "raylib.h"
#include "EntityType.hpp"
#include <string_view>

class Projectile
{
public:
    Projectile();
    ~Projectile();
    Projectile(const Projectile& other);
    Projectile& operator=(const Projectile& other);
    Projectile(Projectile&& other) noexcept;
    Projectile& operator=(Projectile&& other) noexcept;

    void Update();
    void Render() const noexcept;

    Vector2 GetPosition() const noexcept { return position; }
    void SetPosition(Vector2 newPosition) noexcept { position = newPosition; }

    bool IsActive() const noexcept { return active; }
    void SetActive(bool isActive) noexcept { active = isActive; }

    int GetSpeed() const noexcept { return speed; }
    void SetSpeed(int newSpeed) noexcept { speed = newSpeed; }

    EntityType GetType() const noexcept { return type; }
    void SetType(EntityType newType) noexcept { type = newType; }

    Vector2 GetLineStart() const noexcept { return lineStart; }
    Vector2 GetLineEnd() const noexcept { return lineEnd; }

private:
    Vector2 position = { 0, 0 };
    int speed = 15;
    bool active = true;
    EntityType type;

    Vector2 lineStart = { 0, 0 };
    Vector2 lineEnd = { 0, 0 };

    static Texture2D texture;
    static int instanceCount;
};