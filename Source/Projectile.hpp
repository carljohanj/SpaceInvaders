#pragma once
#include "raylib.h"
#include "EntityType.hpp"
#include "TextureWrapper.hpp"
#include <string_view>

class Projectile
{
public:
    Projectile();
    Projectile(const Projectile&) = delete;
    Projectile& operator=(const Projectile&) = delete;
    Projectile(Projectile&& other) noexcept;
    Projectile& operator=(Projectile&& other) noexcept;
    void Update();
    void Render() const noexcept;

    Vector2 GetPosition() const noexcept { return position; }
    bool IsActive() const noexcept { return active; }
    int GetSpeed() const noexcept { return speed; }
    ProjectileType GetType() const noexcept { return type; }
    Vector2 GetLineStart() const noexcept { return lineStart; }
    Vector2 GetLineEnd() const noexcept { return lineEnd; }

    void SetPosition(Vector2 newPosition) noexcept { position = newPosition; }
    void SetSpeed(int newSpeed) noexcept { speed = newSpeed; }
    void SetType(ProjectileType newType) noexcept { type = newType; }
    void SetActive(bool isActive) noexcept { active = isActive; }

private:
    Vector2 position = { 0, 0 };
    int speed = 15;
    bool active = true;
    ProjectileType type;

    Vector2 lineStart = { 0, 0 };
    Vector2 lineEnd = { 0, 0 };

    TextureWrapper texture;
};