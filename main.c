//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>

#include "engine.h"
#include "game/Player.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
    Engine engine;
    if (!InitEngine("GAME", SCREEN_WIDTH, SCREEN_HEIGHT, 4, &engine))
        return false;

    Player player;
    PlayerInit(&player, &engine);

    SDL_Texture* background = LoadTexture("assets/background.png", engine.renderer);
    SDL_QueryTexture(background, NULL, NULL, &engine.levelWidth, &engine.levelHeight);

    SDL_Rect srcRect;
    SDL_Rect destRect;
    bool quit = false;
    char fps[10];
    SDL_Event e;
    while (!quit)
    {
        EngineUpdate(&engine);

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            EngineHandleEvent(&engine, &e);
        }
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        PlayerHandleInput(&player, &engine, currentKeyStates);
        PlayerUpdate(&player, &engine);

        EngineUpdateCamera(&engine, player.pos.x, player.pos.y, player.anim.w, player.anim.h);

        SDL_RenderClear(engine.renderer);

        EngineGetScreenRect(&engine, &destRect, 0, 0, engine.levelWidth, engine.levelHeight);
        SDL_RenderCopy(engine.renderer, background, NULL, &destRect);

        EngineGetScreenRect(&engine, &destRect, player.pos.x, player.pos.y, player.anim.w, player.anim.h);
        SetAnimRect(&player.anim, &srcRect);
        SDL_RenderCopy(engine.renderer, player.tex, &srcRect, &destRect);

        itoa(engine.fps, fps, 10);
        RenderText(&engine, fps, (SDL_Color){ 0, 0, 0, 255 }, 0, 0);

        SDL_RenderPresent(engine.renderer);
    }
    SDL_DestroyTexture(background);
    background = NULL;
    
    PlayerFree(&player);
    CloseEngine(&engine);
    return 0;
}