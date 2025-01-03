#include "GameWindow.hpp"
#include "GameWindowException.hpp"
#include "Config.hpp"
#include <complex>

inline constexpr int startTitleXPos = 200;
inline constexpr int startTitleYPos = 100;
inline constexpr int startSubtitleXPos = 200;
inline constexpr int startSubtitleYPos = 350;
inline constexpr int textBoxWidth = 225;
inline constexpr int textBoxHeight = 50;
inline constexpr int textBoxX = 600;
inline constexpr int textBoxY = 500;

GameWindow::GameWindow()
{
    InitWindow(Config::screenWidth, Config::screenHeight, Config::title.data());
    if (!IsWindowReady())
    {
        throw GameWindowException("Window failed to initialize properly.");
    }
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
    DrawText(Config::title.data(), startTitleXPos, startTitleYPos, Config::startScreenTitleFontSize, YELLOW);
    DrawText(Config::beginMessage.data(), startSubtitleXPos, startSubtitleYPos, Config::startScreenSubtitleFontSize, YELLOW);
}

void GameWindow::RenderGameOverScreen() const noexcept
{
    DrawText(Config::continueMessage.data(), textBoxX, textBoxY, Config::endScreenFontSize, YELLOW);
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