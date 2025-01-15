#pragma once
#include "raylib.h"
#include "EntityType.hpp"

class Projectile
{
public:
    Projectile(Vector2 position, float speed, ProjectileType type, const Texture2D& sharedTexture) noexcept;
    Projectile(const Projectile&) = delete;
    Projectile& operator=(const Projectile&) = delete;
    Projectile(Projectile&& other) noexcept;
    Projectile& operator=(Projectile&& other) noexcept;
    ~Projectile();

    void Update() noexcept;
    void Render() const noexcept;

    Vector2 GetPositionForCollision() const noexcept;
    Vector2 GetPosition() const noexcept;
    bool IsActive() const noexcept;
    int GetSpeed() const noexcept;
    ProjectileType GetType() const noexcept;
    Vector2 GetLineStart() const noexcept;
    Vector2 GetLineEnd() const noexcept;

    void SetPosition(Vector2 newPosition) noexcept;
    void SetSpeed(int newSpeed) noexcept;
    void SetType(ProjectileType newType) noexcept;
    void SetActive(bool isActive) noexcept;

private:
    Vector2 position;
    int speed;
    bool active;
    ProjectileType type;
    Vector2 lineStart;
    Vector2 lineEnd;
    const Texture2D* texture;
};