#include "Player.hpp"
#include <algorithm>
#include "Config.hpp"

Player::Player()
    : gun(Config::playerProjectileSpeed, ProjectileType::PLAYER_PROJECTILE, { 0, -10 }),
    texture(Config::playerTexturePath, Config::playerWidth, Config::playerHeight)
{
    x_pos = GetScreenWidth() / 2;
}

void Player::Reset() noexcept
{
    x_pos = GetScreenWidth() / 2;
    lives = Config::playerStartingLives;
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
        { x_pos, GetScreenHeight() - player_base_height, (float)Config::playerWidth, (float)Config::playerHeight },
        { Config::playerWidth / 2.0f, Config::playerHeight / 2.0f },
        0,
        WHITE);
}

Vector2 Player::GetPosition() const noexcept
{
    return { x_pos, GetScreenHeight() - player_base_height };
}

Projectile Player::Shoot() const noexcept
{
    return gun.Shoot({ x_pos, GetScreenHeight() - player_base_height });
}