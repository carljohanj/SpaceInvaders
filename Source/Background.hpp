#pragma once
#include "raylib.h"
#include "Star.hpp"
#include "Config.hpp"
#include <vector>

class Background
{
public:
    Background() noexcept;
    void Reset() noexcept;
    void Update(float offset) noexcept;
    void Render() const noexcept;

private:
    std::vector<Star> Stars;
    int starAmount;
    int xMinOffset;
    int xMaxOffset;
    float bgSpeed;
};