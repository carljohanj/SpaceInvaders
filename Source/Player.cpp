#include "Player.hpp"
#include <algorithm>

// Constructor
Player::Player()
{
    texture = LoadTexture("./Assets/Ship1.png");
}

// Destructor
Player::~Player()
{
    UnloadTexture(texture);
}

void Player::Initialize()
{
    x_pos = GetScreenWidth() / 2;
}

void Player::Update()
{
    if (IsKeyDown(KEY_LEFT)) { x_pos -= speed; }
    if (IsKeyDown(KEY_RIGHT)) { x_pos += speed; }
    x_pos = std::clamp(x_pos, radius, (float)GetScreenWidth() - radius);
}

void Player::Render()
{
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { x_pos, GetScreenHeight() - player_base_height, 100, 100 },
        { 50, 50 },
        0,
        WHITE);
}

Projectile Player::Shoot()
{
    Projectile projectile;
    projectile.position = { x_pos, GetScreenHeight() - player_base_height - 10 };
    projectile.speed = -15; // Player projectiles move upward
    projectile.type = EntityType::PLAYER_PROJECTILE;
    projectile.active = true;
    return projectile;
}
