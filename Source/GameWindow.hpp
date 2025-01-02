#pragma once
#include "raylib.h"
#include "Background.hpp"
#include <string>
#include "Config.hpp"

class GameWindow
{
public:
    GameWindow();
    ~GameWindow();

    void BeginDrawing() const noexcept;
    void EndDrawing() const noexcept;
    void RenderStartScreen() const noexcept;
    void RenderGameOverScreen() const noexcept;
    void RenderHUD(int score, int playerLives) const noexcept;
    void UpdateBackground(float playerPos) noexcept;
    void RenderBackground() const noexcept;
    void ResetBackground() noexcept;

private:
    Background background;
};