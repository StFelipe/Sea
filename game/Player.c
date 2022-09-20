#include "Player.h"
#include "engine.h"
#include "Util.h"

#include <SDL.h>
#include <SDL_image.h>

void PlayerInit(Player* player, Engine* engine)
{
    player->tex = LoadTexture("assets/character.png", engine->renderer);
    SDL_QueryTexture(player->tex, NULL, NULL, &player->texWidth, &player->texHeight);
    
    player->anim.w = 16;
    player->anim.h = 16;

    player->anim.frame = 0;
    player->anim.frames = 3;
    player->anim.defaultFrame = 1;
    player->anim.speed = 0.1;
    player->anim.timer = 0;
    player->anim.isAnimating = false;

    player->anim.up = 3;
    player->anim.down = 0;
    player->anim.left = 1;
    player->anim.right = 2;
    player->anim.dir = player->anim.down;

    player->pos.x = 100;
    player->pos.y = 100;
}

void PlayerFree(Player* player)
{
    SDL_DestroyTexture(player->tex);
    player->tex = NULL;
}


void PlayerHandleInput(Player* player, Engine* engine, Level* level)
{
    SDL_FPoint direction = { 0, 0 };
    if(IsKeyDown(engine->input, SDL_SCANCODE_LEFT))
    {
        direction.x = -1;
        player->anim.dir = player->anim.left;
    }
    else if(IsKeyDown(engine->input, SDL_SCANCODE_RIGHT))
    {
        direction.x = 1;
        player->anim.dir = player->anim.right;
    }
    if(IsKeyDown(engine->input, SDL_SCANCODE_UP))
    {
        direction.y = -1;
        player->anim.dir = player->anim.up;
    }
    else if(IsKeyDown(engine->input, SDL_SCANCODE_DOWN))
    {
        direction.y = 1;
        player->anim.dir = player->anim.down;
    }
    
    bool isMoving = GetPointSqrMagnitude(&direction) >= 0.001f;
    player->anim.isAnimating = isMoving;
    if (isMoving)
    {
        NormalizePoint(&direction);
        MulPoint(&direction, PLAYER_SPEED * engine->deltaTime);
        AddPoint(&player->pos, &direction);
    }
    else
    {
        player->anim.frame = player->anim.defaultFrame;
    }

    if (player->pos.x < 0)
        player->pos.x = 0;
    else if (player->pos.x > level->w - player->anim.w)
        player->pos.x = level->w - player->anim.w;
    if (player->pos.y < 0)
        player->pos.y = 0;
    else if (player->pos.y > level->h - player->anim.h)
        player->pos.y =  level->h - player->anim.h;
}

void PlayerUpdate(Player* player, Engine* engine)
{
    Anim* anim = &player->anim;
    if (anim->isAnimating)
    {
        anim->timer += engine->deltaTime;
        if (anim->timer >= anim->speed)
        {
            anim->timer = 0;
            anim->frame = (anim->frame + 1) % anim->frames;
        }
    }
}