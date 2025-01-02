#include "GameWindow.hpp"
#include "Config.hpp"
#include <complex>

GameWindow::GameWindow()
{
    InitWindow(Config::screenWidth, Config::screenHeight, Config::title.data());
    SetTargetFPS(60);
}

GameWindow::~GameWindow()
{
    if (IsWindowReady()) { CloseWindow(); }
}

void GameWindow::BeginDrawing() const noexcept
{
    ::BeginDrawing();
    ClearBackground(BLACK);
}

void GameWindow::EndDrawing() const noexcept { ::EndDrawing(); }

void GameWindow::RenderStartScreen() const noexcept
{
    DrawText(Config::title.data(), 200, 100, Config::startScreenTitleFontSize, YELLOW);
    DrawText(Config::beginMessage.data(), 200, 350, Config::startScreenSubtitleFontSize, YELLOW);
}

void GameWindow::RenderGameOverScreen() const noexcept
{
    DrawText(Config::continueMessage.data(), Config::textBoxX, 200, Config::endScreenFontSize, YELLOW);
}

void GameWindow::RenderHUD(int score, int playerLives) const noexcept
{
    DrawText(TextFormat("Score: %i", score), 50, 20, Config::gameplayScoreFontSize, YELLOW);
    DrawText(TextFormat("Lives: %i", playerLives), 50, 70, Config::gameplayLivesFontSize, YELLOW);
}

void GameWindow::UpdateBackground(float playerPos) noexcept
{
    const Vector2 playerPosition = { playerPos, Config::playerBaseHeight };
    const Vector2 screenCorner = { 0.0f, Config::playerBaseHeight };
    const auto dx = screenCorner.x - playerPosition.x;
    const auto dy = screenCorner.y - playerPosition.y;
    const auto backgroundOffset = std::sqrt(dx * dx + dy * dy) * -1.0f;
    background.Update(backgroundOffset / Config::backgroundSpeed);
}

void GameWindow::RenderBackground() const noexcept { background.Render(); }

void GameWindow::ResetBackground() noexcept { background.Reset(); }