#pragma once
#include "StackManager.hpp"

class Game
{
public:
    Game(int screenWidth, int screenHeight);
    void Run();

private:
    struct Private;
    StackManager<700> impl;
};