#include "GameWindow.hpp"

GameWindow::GameWindow(std::string_view gameTitle, int width, int height)
{
    InitWindow(width, height, gameTitle.data());
    SetTargetFPS(60);
}

GameWindow::~GameWindow() 
{ 
    if (IsWindowReady()) { CloseWindow(); }
}

void GameWindow::BeginDrawing() const noexcept { ::BeginDrawing(); }
void GameWindow::EndDrawing()   const noexcept { ::EndDrawing(); }   