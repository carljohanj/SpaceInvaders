#pragma once
#include "StackManager.hpp"

class Game
{
public:
    Game();
    void Run();

private:
    struct Private;
    StackManager<500> impl;
};