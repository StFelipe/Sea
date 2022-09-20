#include "Game.h"

void InitGame(Game* game, Engine* engine)
{
    PlayerInit(&game->player, engine);
    CameraInit(&game->camera, engine);

    game->uiOption.selection = PauseMenuContinue;
    game->uiOption.normalTextColor = NORMAL_TEXT_COLOR;
    game->uiOption.selectedTextColor = SELECTED_TEXT_COLOR;

    game->background = LoadTexture("assets/background.png", engine->renderer);
    SDL_QueryTexture(game->background, NULL, NULL, &game->level.w, &game->level.h);
}

void FreeGame(Game* game)
{
    PlayerFree(&game->player);
    
    SDL_DestroyTexture(game->background);
    game->background = NULL;
}

void GameHandleEvent(Game* game, Engine* engine, SDL_Event* e)
{
    if (e->type == SDL_WINDOWEVENT)
    {
        if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            SetCameraScale(&game->camera, engine, game->level.w, game->level.h);
    }

    if (engine->isPaused)
        return;

    if (e->type == SDL_MOUSEWHEEL)
    {
        if(e->wheel.y > 0)
            CameraZoomIn(&game->camera, engine, game->level.w, game->level.h);
        else if(e->wheel.y < 0)
            CameraZoomOut(&game->camera, engine, game->level.w, game->level.h);
    }
}

void UpdateGame(Game* game, Engine* engine)
{
    if (IsKeyPresed(engine->input, SDL_SCANCODE_ESCAPE))
        engine->isPaused = !engine->isPaused;

    Player* player = &game->player;

    if (!engine->isPaused)
    {
        PlayerHandleInput(player, engine, &game->level);
        PlayerUpdate(player, engine);

        UpdateCamera(&game->camera, player->pos.x, player->pos.y, player->anim.w, player->anim.h, game->level.w, game->level.h);
    }

    SDL_RenderClear(engine->renderer);

    GetScreenRectFromCamera(&game->camera, &game->destRect, 0, 0, game->level.w, game->level.h);
    SDL_RenderCopy(engine->renderer, game->background, NULL, &game->destRect);

    GetScreenRectFromCamera(&game->camera, &game->destRect, player->pos.x, player->pos.y, player->anim.w, player->anim.h);
    SetAnimRect(&player->anim, &game->srcRect);
    SDL_RenderCopy(engine->renderer, player->tex, &game->srcRect, &game->destRect);

    itoa(engine->fps, game->fps, 10);
    RenderText(engine, game->fps, (SDL_Color){ 0, 0, 0, 255 }, 0, 0);

    if (engine->isPaused)
    {
        if (IsKeyPresed(engine->input, SDL_SCANCODE_RETURN))
        {
            if (game->uiOption.selection == PauseMenuContinue)
                engine->isPaused = false;
                
            if (game->uiOption.selection == PauseMenuBack)
            {
                engine->isPaused = false;
                engine->state = MainMenuState;
            }
        }

        UiCheckUpDown(&game->uiOption, engine, PauseMenuBack);

        game->destRect.x = 0;
        game->destRect.y = 0;
        game->destRect.w = engine->windowWidth;
        game->destRect.h = engine->windowHeight;
        
        SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(engine->renderer, &game->destRect);
        SDL_SetRenderDrawBlendMode(engine->renderer, SDL_BLENDMODE_NONE);

        int x = 100;
        int y = (engine->windowHeight / 4);
        RenderText(engine, "Continue", GetColorOnSelection(&game->uiOption, PauseMenuContinue), x, y);
        RenderText(engine, "Main Menu", GetColorOnSelection(&game->uiOption, PauseMenuBack), x, y + 50);
    }

    SDL_RenderPresent(engine->renderer);
}