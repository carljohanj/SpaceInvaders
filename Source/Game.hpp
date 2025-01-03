#pragma once
#include "StackManager.hpp"

class Game
{
public:
    Game();
    void Run();

private:
    struct Private;
    StackManager<300> impl;
};