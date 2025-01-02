#include "Player.hpp"
#include <algorithm>
#include "Config.hpp"

Player::Player()
    : gun(-15.0f, EntityType::PLAYER_PROJECTILE, { 0, -10 }),
      texture(Config::playerTexturePath)
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
    return gun.Shoot({ x_pos, GetScreenHeight() - player_base_height });
}

Vector2 Player::GetPosition() const noexcept
{
    return { x_pos, GetScreenHeight() - Config::playerBaseHeight };
}