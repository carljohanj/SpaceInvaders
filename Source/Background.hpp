#pragma once
#include "raylib.h"
#include "Star.hpp"
#include <vector>

class Background 
{
public:
    std::vector<Star> Stars;

    void Initialize(int starAmount);
    void Update(float offset);
    void Render();
};
