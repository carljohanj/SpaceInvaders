#pragma once
#include "raylib.h"
#include <string>

class GameWindow
{
public:
    GameWindow(int screenWidth, int screenHeight);
    ~GameWindow() noexcept;
    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;
    GameWindow(GameWindow&&) noexcept = default;
    GameWindow& operator=(GameWindow&&) noexcept = default;

    void BeginDrawing() const noexcept;
    void EndDrawing() const noexcept;
    void RenderStartScreen() const noexcept;
    void RenderHUD(int score, int playerLives) const noexcept;
    void RenderGameOverScreen() const noexcept;
};