#include "raylib.h"
#include "GameWindow.hpp"
#include "WindowConfig.hpp"
#include "Game.hpp"

int main(void) {
    GameWindow window{ title, screenWidth, screenHeight };
    Game game = { State::STARTSCREEN };

    Resources resources;
    game.resources = resources;
    game.Launch();

    InitAudioDevice();
    auto sound = LoadSound("./hitHurt.ogg");

    while (!WindowShouldClose()) {
        game.Update();

        window.BeginDrawing();
        ClearBackground(BLACK);
        game.Render();
        window.EndDrawing();
    }

    CloseAudioDevice();
    return 0; // GameWindow destructor will handle CloseWindow
}