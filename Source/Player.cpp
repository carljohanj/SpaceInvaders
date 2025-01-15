#include "Player.hpp"
#include <algorithm>
#include "Config.hpp"
#include "Utilities.hpp"

[[gsl::suppress(f .6, justification: "We want the constructor to throw if the texture can't be loaded")]]
Player::Player()
    : gun(Config::playerProjectileSpeed, ProjectileType::PLAYER_PROJECTILE, { 0, -10 }),
    texture(Config::playerTexturePath, Config::playerWidth, Config::playerHeight)
{
    x_pos = GetScreenWidth() / 2.0f;
}

void Player::Reset() noexcept
{
    x_pos = GetScreenWidth() / 2.0f;
    lives = Config::playerStartingLives;
}

void Player::Update() noexcept
{
    if (IsKeyDown(KEY_LEFT)) { x_pos -= speed; }
    if (IsKeyDown(KEY_RIGHT)) { x_pos += speed; }
    x_pos = std::clamp(x_pos, radius, (float)GetScreenWidth() - radius);
}

void Player::Render() const 
{
    Util::RenderRectangle(texture.GetTexture(), { x_pos, GetScreenHeight() - player_base_height },
                    Config::playerWidth, Config::playerHeight);
}

Vector2 Player::GetPosition() const noexcept { return { x_pos, GetScreenHeight() - player_base_height }; }

Projectile Player::Shoot() const noexcept { return gun.Shoot({ x_pos, GetScreenHeight() - player_base_height }); }