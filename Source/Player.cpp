#include "Player.hpp"
#include <algorithm>
#include "Config.hpp"

Player::Player()
    : texture(Config::playerTexturePath)
{
    x_pos = GetScreenWidth() / 2;
}

void Player::Reset() noexcept
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
    DrawTexturePro(texture.GetTexture(),
        { 0, 0, (float)texture.GetTexture().width, (float)texture.GetTexture().height },
        { x_pos, GetScreenHeight() - player_base_height, 100, 100 },
        { 50, 50 },
        0,
        WHITE);
}

Projectile Player::Shoot() const noexcept
{
    Projectile projectile;
    projectile.SetPosition({ x_pos, GetScreenHeight() - player_base_height - 10 });
    projectile.SetSpeed(-15);
    projectile.SetType(EntityType::PLAYER_PROJECTILE);
    projectile.SetActive(true);
    return projectile;
}