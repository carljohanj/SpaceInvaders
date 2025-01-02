#include "Game.hpp"
#include "TextureLoadingException.hpp"
#include "GameWindowException.hpp"
#include <iostream>
#include <print>

int main()
{
    try
    {
        Game game;
        game.Run();
    }
    catch (const TextureLoadingException& e)
    {
        std::println("Exception caught:\n{}:", e.what());
    }
    return 0;
}
