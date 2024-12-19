#include "GameWindow.hpp"

GameWindow::GameWindow(const char* gameTitle, int width, int height)
{
    InitWindow(width, height, gameTitle);
    SetTargetFPS(60);
}

GameWindow::~GameWindow() {
    if (IsWindowReady()) {
        CloseWindow();
    }
}

void GameWindow::BeginDrawing() const noexcept {
    ::BeginDrawing();
}

void GameWindow::EndDrawing() const noexcept {
    ::EndDrawing();
}   