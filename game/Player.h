#ifndef PLAYER_H
#define PLAYER_H

#include "engine.h"
#include "Level.h"
#include "Util.h"
#include <SDL.h>
#include <SDL_image.h>

#define PLAYER_SPEED 100

typedef struct Player {
    SDL_Texture* tex;
    int texWidth;
    int texHeight;
    Anim anim;
    SDL_FPoint pos;
} Player;

void PlayerInit(Player* player, Engine* engine);
void PlayerFree(Player* player);

void PlayerHandleInput(Player* player, Engine* engine, Level* level);
void PlayerUpdate(Player* player, Engine* engine);

#endif