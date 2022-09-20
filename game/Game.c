#include "Game.h"

void InitGame(Game* game, Engine* engine)
{
    PlayerInit(&game->player, engine);

    game->background = LoadTexture("assets/background.png", engine->renderer);
    SDL_QueryTexture(game->background, NULL, NULL, &engine->levelWidth, &engine->levelHeight);
}

void FreeGame(Game* game)
{
    PlayerFree(&game->player);
    
    SDL_DestroyTexture(game->background);
    game->background = NULL;
}

void GameHandleEvent(Game* game, Engine* engine, SDL_Event* e)
{
    EngineHandleEvent(engine, e);
}

void UpdateGame(Game* game, Engine* engine, const Uint8* keyStates)
{
    Player* player = &game->player;

    PlayerHandleInput(player, engine, keyStates);
    PlayerUpdate(player, engine);

    EngineUpdateCamera(engine, player->pos.x, player->pos.y, player->anim.w, player->anim.h);

    SDL_RenderClear(engine->renderer);

    EngineGetScreenRect(engine, &game->destRect, 0, 0, engine->levelWidth, engine->levelHeight);
    SDL_RenderCopy(engine->renderer, game->background, NULL, &game->destRect);

    EngineGetScreenRect(engine, &game->destRect, player->pos.x, player->pos.y, player->anim.w, player->anim.h);
    SetAnimRect(&player->anim, &game->srcRect);
    SDL_RenderCopy(engine->renderer, player->tex, &game->srcRect, &game->destRect);

    itoa(engine->fps, game->fps, 10);
    RenderText(engine, game->fps, (SDL_Color){ 0, 0, 0, 255 }, 0, 0);

    SDL_RenderPresent(engine->renderer);
}