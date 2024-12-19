#include "Alien.hpp"

Alien::Alien() : position{ 0, 0 }, radius(30), active(true), moveRight(true), speed(2) {
    type = EntityType::ENEMY;
}

// Update Alien movement
void Alien::Update() {
    if (moveRight) {
        position.x += speed;
        if (position.x >= GetScreenWidth()) {
            moveRight = false;
            position.y += 50;
        }
    }
    else {
        position.x -= speed;
        if (position.x <= 0) {
            moveRight = true;
            position.y += 50;
        }
    }
}

// Render Alien
void Alien::Render(Texture2D texture) 
{
    DrawTexturePro(texture,
        { 0, 0, 352, 352 },
        { position.x, position.y, 100, 100 },
        { 50 , 50 },
        0,
        WHITE);
}

Projectile Alien::Shoot() {
    Projectile newProjectile;
    newProjectile.position = { position.x, position.y + 40 };
    newProjectile.speed = 5;
    newProjectile.type = EntityType::ENEMY_PROJECTILE;
    newProjectile.active = true;
    return newProjectile;
}
