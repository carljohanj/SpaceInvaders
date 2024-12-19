#pragma once
#include "raylib.h"

struct Star
{
    Vector2 initPosition = { 0, 0 };
    Vector2 position = { 0, 0 };
    Color color = GRAY;
    float size = 0;

    void Update(float starOffset) noexcept;
    void Render() const noexcept;
};