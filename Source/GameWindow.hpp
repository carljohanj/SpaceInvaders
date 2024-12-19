#pragma once
#include "raylib.h"
#include <string>

class GameWindow {
public:
    GameWindow(const char* title, int width, int height);
    ~GameWindow();
    void BeginDrawing() const noexcept;
    void EndDrawing() const noexcept;
};