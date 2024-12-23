#pragma once
#include "raylib.h"
#include "Projectile.hpp"
#include "EntityType.hpp"
#include <iostream>

class Alien 
{
public:
    Alien();
    Alien(const Alien&);
    Alien(Alien&& other) noexcept;
    ~Alien();
    Alien& operator=(const Alien&);
    Alien& operator=(Alien&& other) noexcept;
    void Update();
    void Render() const noexcept;
    Projectile Shoot();
    void SetPosition(Vector2 pos) noexcept { position = pos; }
    Vector2 GetPosition() const noexcept { return position; }
    float GetRadius() const noexcept { return radius; }
    bool IsActive() const noexcept { return active; }
    void SetActive(bool isActive) noexcept { active = isActive; }

private:
    Vector2 position;
    float radius = 20.0f;
    float speed = 2.0f;
    bool active = true;
    bool moveRight = true;
    static Texture2D texture;
    static int instanceCount;
};