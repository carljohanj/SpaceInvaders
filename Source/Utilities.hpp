#pragma once
#include "raylib.h"
#include <string_view>

namespace Util
{
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

    inline void DrawRectangleLines(Rectangle rect, Color color) noexcept
    {
        ::DrawRectangleLines(static_cast<int>(rect.x),
            static_cast<int>(rect.y),
            static_cast<int>(rect.width),
            static_cast<int>(rect.height),
            color);
    }

    inline float GetRandomValue(int min, int max) noexcept
    {
        return static_cast<float>(::GetRandomValue(min, max));
    }

    inline void DrawCircle(Vector2 position, float radius, Color color) noexcept 
    {
        ::DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), radius, color);
    }

}