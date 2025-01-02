#pragma once
#include "raylib.h"
#include "TextureWrapper.hpp"

class Wall
{
public:
    Wall();
    Wall(Wall&& other) noexcept;
    Wall& operator=(Wall&& other) noexcept;
    Wall(Wall& other) = delete;
    Wall& operator=(Wall& other) = delete;
    ~Wall();
    Wall(const Wall&) = delete;
    Wall& operator=(const Wall&) = delete;
    void Render() noexcept;
    Rectangle GetRectangle() const noexcept;
    Vector2 GetPosition() const noexcept { return position; }
    void SetPosition(Vector2 newPosition) noexcept { position = newPosition; }
    bool IsActive() const noexcept { return active; }
    void SetActive(bool isActive) noexcept { active = isActive; }
    int GetHealth() const noexcept { return health; }
    void SetHealth(int newHealth) noexcept { health = newHealth; }
    int GetRadius() const noexcept { return radius; }

private:
    void RenderWallText() const noexcept;
    void RenderHealth() noexcept;
    Vector2 position = { 0, 0 };
    bool active = true;
    int health = 50;
    float radius = 60;

    TextureWrapper texture;
};