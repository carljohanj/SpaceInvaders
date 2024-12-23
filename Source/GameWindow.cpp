#include "GameWindow.hpp"
#include "Config.hpp"

GameWindow::GameWindow()
{
    InitWindow(Config::screenWidth, Config::screenHeight, Config::title.data());
    SetTargetFPS(60);
}

GameWindow::~GameWindow() 
{ 
    if (IsWindowReady()) { CloseWindow(); }
}

void GameWindow::BeginDrawing() const noexcept { ::BeginDrawing(); }
void GameWindow::EndDrawing()   const noexcept { ::EndDrawing(); }   