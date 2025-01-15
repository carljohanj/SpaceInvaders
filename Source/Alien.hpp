#pragma once
#include "Gun.hpp"
#include "raylib.h"
#include "Projectile.hpp"
#include "EntityType.hpp"
#include "TextureWrapper.hpp"

class Alien
{
public:
    Alien(float x, float y);
    Alien(const Alien&) = delete;
    Alien(Alien&& other) noexcept;
    ~Alien();
    Alien& operator=(const Alien&) = delete;
    Alien& operator=(Alien&& other) noexcept;

    void Update() noexcept;
    void Render() const;
    Projectile Shoot() const noexcept;

    void SetPosition(Vector2 pos) noexcept;
    Vector2 GetPosition() const noexcept;
    float GetRadius() const noexcept;
    bool IsActive() const noexcept;
    void SetActive(bool isActive) noexcept;

private:
    void MoveToRight() noexcept;
    void MoveToLeft() noexcept;
    void MoveDown() noexcept;

    Vector2 position;
    float radius;
    float speed;
    bool active;
    bool moveRight;
    Gun gun;
    TextureWrapper texture;
};