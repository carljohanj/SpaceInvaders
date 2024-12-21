// Wall.hpp

#pragma once
#include "raylib.h"

class Wall
{
public:
    Wall();
    ~Wall();
    void Update();
    void Render() const noexcept;
    Vector2 GetPosition() const noexcept { return position; }
    void SetPosition(Vector2 newPosition) noexcept { position = newPosition; }
    bool IsActive() const noexcept { return active; }
    void SetActive(bool isActive) noexcept { active = isActive; }
    int GetHealth() const noexcept { return health; }
    void SetHealth(int newHealth) noexcept { health = newHealth; }
    int GetRadius() const noexcept { return radius; }
    static void IncrementInstanceCount();
    static void DecrementInstanceCount();
    static int GetInstanceCount();

private:
    Vector2 position = { 0, 0 };
    bool active = true;
    int health = 50;
    int radius = 60;

    static Texture2D texture;
    static int instanceCount;
};