#include "GameWindow.hpp"
#include "Config.hpp"

GameWindow::GameWindow()
{
    InitWindow(screenWidth, screenHeight, title.data());
    SetTargetFPS(60);
}

GameWindow::~GameWindow() 
{ 
    if (IsWindowReady()) { CloseWindow(); }
}

void GameWindow::BeginDrawing() const noexcept { ::BeginDrawing(); }
void GameWindow::EndDrawing()   const noexcept { ::EndDrawing(); }   