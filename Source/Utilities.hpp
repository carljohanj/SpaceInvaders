#pragma once
#include "raylib.h"

inline void RenderRectangle(const Texture2D& texture, Vector2 position, int width, int height) noexcept
{
    DrawTexturePro(texture,
        { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
        { position.x, position.y, static_cast<float>(width), static_cast<float>(height) },
        { static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f },
        0.0f,
        WHITE);
}

inline void RenderCircle(const Texture2D& texture, Vector2 position, float radius) noexcept 
{
    DrawTexturePro(texture,
        { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
        { position.x, position.y, radius * 2.0f, radius * 2.0f },
        { radius, radius },
        0.0f,
        WHITE);
}