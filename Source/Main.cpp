#include "raylib.h"
#include "Game.h"

int main(void)
{    
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;       //ToDo: Place magic value as constant in struct in WindowConfig header instead
    const int screenHeight = 1080;      //ToDo: Place magic value as constant in struct in WindowConfig header instead

    InitWindow(screenWidth, screenHeight, "SPACE INVADERS");    //ToDo: Create Window class to handle window init and cleanup
                                                                //ToDo: Also use composition and place Window inside Game

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Game game = { State::STARTSCREEN };
    Resources resources;
    game.resources = resources;
    game.Launch();

    
    //--------------------------------------------------------------------------------------

    InitAudioDevice();              //ToDo: Create AudioManager class to handle loading and unloading of sounds

    auto sound = LoadSound("./hitHurt.ogg");
    


    //ToDo: Move main game loop into Game.run() instead
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        game.Update();
      

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

       

        game.Render();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseAudioDevice();
    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}