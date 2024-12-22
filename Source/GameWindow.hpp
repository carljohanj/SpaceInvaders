#pragma once
#include "raylib.h"
#include <string>

class GameWindow 
{
public:
    GameWindow(std::string_view title, int width, int height);
    ~GameWindow();
    void BeginDrawing() const noexcept;
    void EndDrawing() const noexcept;
};