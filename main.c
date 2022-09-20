//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>

#include "engine.h"
#include "game/Player.h"
#include "game/Game.h"
#include "game/Editor.h"
#include "game/MainMenu.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
    Engine engine;
    if (!InitEngine("GAME", SCREEN_WIDTH, SCREEN_HEIGHT, 4, &engine))
        return false;

    Game game;
    InitGame(&game, &engine);

    MainMenu mainMenu;
    InitMainMenu(&mainMenu, &engine);

    Editor editor;
    InitEditor(&editor, &engine);

    SDL_Rect srcRect;
    SDL_Rect destRect;
    bool quit = false;
    SDL_Event e;
    State state = MainMenuState;

    while (!quit)
    {
        EngineUpdate(&engine);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;

            GameHandleEvent(&game, &engine, &e);
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        UpdateGame(&game, &engine, currentKeyStates);
    }
    
    FreeGame(&game);
    FreeMainMenu(&mainMenu);
    FreeEditor(&editor);
    
    CloseEngine(&engine);
    return 0;
}