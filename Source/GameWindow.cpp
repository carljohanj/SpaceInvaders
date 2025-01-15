#include "GameWindow.hpp"
#include "GameWindowException.hpp"
#include "Config.hpp"

namespace
{
    constexpr int startTitleXPos = 200;
    constexpr int startTitleYPos = 100;
    constexpr int startSubtitleXPos = 200;
    constexpr int startSubtitleYPos = 350;
    constexpr int textBoxWidth = 225;
    constexpr int textBoxHeight = 50;
    constexpr int textBoxX = 600;
    constexpr int textBoxY = 500;
}

GameWindow::GameWindow(int screenWidth, int screenHeight)
{
    InitWindow(screenWidth, screenHeight, Config::title.data());
    if (!IsWindowReady())
    {
        throw GameWindowException("Window failed to initialize properly.");
    }
    SetTargetFPS(60);
}

GameWindow::~GameWindow() noexcept { CloseWindow(); }

void GameWindow::BeginDrawing() const noexcept
{
    ::BeginDrawing();
    ::ClearBackground(BLACK);
}

void GameWindow::EndDrawing() const noexcept { ::EndDrawing(); }

void GameWindow::RenderStartScreen() const noexcept
{
    DrawText(Config::title.data(), startTitleXPos, startTitleYPos, Config::startScreenTitleFontSize, YELLOW);
    DrawText(Config::beginMessage.data(), startSubtitleXPos, startSubtitleYPos, Config::startScreenSubtitleFontSize, YELLOW);
}

void GameWindow::RenderHUD(int score, int playerLives) const noexcept
{
    DrawText(TextFormat("Score: %i", score), 50, 20, Config::gameplayScoreFontSize, YELLOW);
    DrawText(TextFormat("Lives: %i", playerLives), 50, 70, Config::gameplayLivesFontSize, YELLOW);
}

void GameWindow::RenderGameOverScreen() const noexcept
{
    DrawText(Config::continueMessage.data(), textBoxX, textBoxY, Config::endScreenFontSize, YELLOW);
}