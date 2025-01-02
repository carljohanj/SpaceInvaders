#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include "Player.hpp"
#include "Alien.hpp"
#include "StackManager.hpp"
#include "Projectile.hpp"
#include "Wall.hpp"
#include "Star.hpp"
#include "Background.hpp"
#include "EntityType.hpp"
#include "GameWindow.hpp"
#include "Config.hpp"

enum class State
{
    STARTSCREEN,
    GAMEPLAY,
    ENDSCREEN
};

struct PlayerData
{
    std::string name;
    int score;
};

class Game
{
public:
    Game();
    void Run();

private:
    struct Private;
    StackManager<256> impl;
};