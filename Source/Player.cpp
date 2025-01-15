#include "Player.hpp"
#include <algorithm>
#include "Config.hpp"
#include "Utilities.hpp"

Player::Player()
    : x_pos(GetScreenWidth() / 2.0f),
    speed(7.0f),
    player_base_height(70.0f),
    radius(50.0f),
    lives(Config::playerStartingLives),
    gun(Config::playerProjectileSpeed, ProjectileType::PLAYER_PROJECTILE, { 0, -10 }),
    texture(Config::playerTexturePath, Config::playerWidth, Config::playerHeight)
{
}

void Player::Reset() noexcept
{
    x_pos = GetScreenWidth() / 2.0f;
    lives = Config::playerStartingLives;
}

void Player::Update() noexcept
{
    if (IsKeyDown(KEY_LEFT))
    {
        x_pos -= speed;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        x_pos += speed;
    }
    x_pos = std::clamp(x_pos, radius, static_cast<float>(GetScreenWidth()) - radius);
}

void Player::Render() const
{
    Util::RenderRectangle(
        texture.GetTexture(),
        { x_pos, static_cast<float>(GetScreenHeight()) - Config::playerBaseHeight },
        Config::playerWidth,
        Config::playerHeight
    );
}

Vector2 Player::GetPosition() const noexcept
{
    return { x_pos, static_cast<float>(GetScreenHeight()) - player_base_height };
}

Projectile Player::Shoot() const noexcept
{
    return gun.Shoot({ x_pos, static_cast<float>(GetScreenHeight()) - player_base_height });
}

float Player::GetXPosition() const noexcept { return x_pos; }

float Player::GetPlayerBaseHeight() const noexcept { return player_base_height; }

float Player::GetRadius() const noexcept { return radius; }

int Player::GetLives() const noexcept { return lives; }

void Player::SetLives(int newLives) noexcept { lives = newLives; }

void Player::SetXPosition(float newXPos) noexcept { x_pos = newXPos; }