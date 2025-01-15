#pragma once
#include "raylib.h"
#include "TextureWrapper.hpp"

class Wall
{
public:
    Wall();
    Wall(Wall&& other) noexcept;
    Wall& operator=(Wall&& other) noexcept;
    Wall(const Wall&) = delete;
    Wall& operator=(const Wall&) = delete;
    ~Wall() = default;

    void Render() const;
    Rectangle GetRectangle() const noexcept;

    Vector2 GetPosition() const noexcept;
    void SetPosition(Vector2 newPosition) noexcept;
    bool IsActive() const noexcept;
    void SetActive(bool isActive) noexcept;
    int GetHealth() const noexcept;
    void SetHealth(int newHealth) noexcept;
    float GetRadius() const noexcept;

private:
    void RenderHealth() const noexcept;
    Vector2 position = { 0, 0 };
    bool active = true;
    int health = 50;
    float radius = 60;
    TextureWrapper texture;
};