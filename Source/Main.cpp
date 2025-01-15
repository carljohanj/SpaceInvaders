#include "Game.hpp"
#include "Config.hpp"
#include "TextureLoadingException.hpp"
#include "GameWindowException.hpp"
#include <iostream>
#include <print>

int main()
{
    try
    {
        Game game(Config::screenWidth, Config::screenHeight);
        game.Run();
    }
    catch (const TextureLoadingException& e)
    {
        std::println("Exception caught:\n{}:", e.what());
    }
    catch (const GameWindowException& e)
    {
        std::println("Exception caught:\n{}:", e.what());
    }

    return 0;
}