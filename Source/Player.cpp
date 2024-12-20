#include "Player.hpp"
#include <algorithm>
#include <TextureLoadingException.hpp>

Player::Player()
{
    texture = LoadTexture("./Assets/Ship1.png");
    if (texture.id == 0) { throw TextureLoadingException("Failed to load Player texture: ./Assets/Ship1.png"); }
}

// Destructor
Player::~Player() 
{
    UnloadTexture(texture); 
}

void Player::RePosition() noexcept
{
    x_pos = GetScreenWidth() / 2;
}

void Player::Update() noexcept
{
    if (IsKeyDown(KEY_LEFT)) { x_pos -= speed; }
    if (IsKeyDown(KEY_RIGHT)) { x_pos += speed; }
    x_pos = std::clamp(x_pos, radius, (float)GetScreenWidth() - radius);
}

void Player::Render() const noexcept
{
    DrawTexturePro(texture,
        { 0, 0, (float)texture.width, (float)texture.height },
        { x_pos, GetScreenHeight() - player_base_height, 100, 100 },
        { 50, 50 },
        0,
        WHITE);
}

Projectile Player::Shoot() const noexcept
{
    Projectile projectile;
    projectile.position = { x_pos, GetScreenHeight() - player_base_height - 10 };
    projectile.speed = -15;
    projectile.type = EntityType::PLAYER_PROJECTILE;
    projectile.active = true;
    return projectile;
}
