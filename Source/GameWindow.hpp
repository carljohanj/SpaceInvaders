#pragma once
#include "raylib.h"
#include <string>
#include "Config.hpp"

class GameWindow 
{
public:
    GameWindow();
    ~GameWindow();
    void BeginDrawing() const noexcept;
    void EndDrawing() const noexcept;
};