#include "Player.hpp"
#include <algorithm>

void Player::Initialize() {
    x_pos = GetScreenWidth() / 2;
}

void Player::Update() {
    if (IsKeyDown(KEY_LEFT)) {
        x_pos -= speed;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        x_pos += speed;
    }

    x_pos = std::clamp(x_pos, radius, (float)GetScreenWidth() - radius);
}

void Player::Render(Texture2D texture) {
    DrawTexturePro(texture,
        { 0, 0, 352, 352 },
        { x_pos, GetScreenHeight() - player_base_height, 100, 100 },
        { 50, 50 },
        0,
        WHITE);
}

Projectile Player::Shoot() {
    Projectile newProjectile;
    newProjectile.position = { x_pos, GetScreenHeight() - player_base_height - 10 };
    newProjectile.speed = -15; // Player projectiles move upward
    newProjectile.type = EntityType::PLAYER_PROJECTILE;
    newProjectile.active = true;
    return newProjectile;
}