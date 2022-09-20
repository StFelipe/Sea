#include "engine.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>


void SetScale(Engine* engine);

void InitManagers(Engine* engine)
{
    engine->window = NULL;
    engine->renderer = NULL;
}

void FreeManagers(Engine* engine)
{
    SDL_DestroyRenderer(engine->renderer);
    engine->renderer = NULL;

    SDL_DestroyWindow(engine->window);
    engine->window = NULL;
}

int InitEngine(const char* windowName, int windowWidth, int windowHeight, int startingZoom, Engine* engine)
{
    engine->windowWidth = windowWidth;
    engine->windowHeight = windowHeight;

    engine->renderStartPoint = (SDL_Point){ 0, 0 };
    engine->camera = (SDL_FRect){ 0, 0, windowWidth / startingZoom, windowHeight / startingZoom };
    engine->zoom = startingZoom;

    engine->lastTicks = 0;
    engine->deltaTime = 0;
    engine->fps = 0;
    engine->fpsIndex = 0;

    InitManagers(engine);
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    engine->window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if( engine->window == NULL )
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_ACCELERATED);
    if(engine->window == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(engine->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if((IMG_Init(imgFlags) & imgFlags) != imgFlags)
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    if( TTF_Init() == -1 )
    {
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return false;
    }

    engine->font = TTF_OpenFont("assets/Monocraft.ttf", 28);
    if(engine->font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

SDL_Texture* LoadTexture(char* path, SDL_Renderer* renderer)
{
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path);
    if(newTexture == NULL)
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());

    return newTexture;
}

void CloseEngine(Engine* engine)
{
    FreeManagers(engine);

    TTF_CloseFont(engine->font);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SetScale(Engine* engine)
{
    int realLevelWidth = engine->levelWidth * engine->zoom;
    int realLevelHeight = engine->levelHeight * engine->zoom;

    if (realLevelWidth <= engine->windowWidth)
        engine->camera.w = realLevelWidth;
    else
        engine->camera.w = engine->windowWidth / engine->zoom;

    if (realLevelHeight <= engine->windowHeight)
        engine->camera.h = realLevelHeight;
    else
        engine->camera.h = engine->windowHeight / engine->zoom;


    engine->renderStartPoint.x = (engine->windowWidth / 2) - (realLevelWidth / 2);
    engine->renderStartPoint.y = (engine->windowHeight / 2) - (realLevelHeight / 2);
    if (engine->renderStartPoint.x < 0)
        engine->renderStartPoint.x = 0;
    if (engine->renderStartPoint.y < 0)
        engine->renderStartPoint.y = 0;
}

void EngineHandleEvent(Engine* engine, const SDL_Event* e)
{
    if (e->type == SDL_WINDOWEVENT)
    {
        switch(e->window.event)
        {
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                engine->windowWidth = e->window.data1;
                engine->windowHeight = e->window.data2;
                SetScale(engine);
                break;
        }
    }
    
    else if (e->type == SDL_MOUSEWHEEL)
    {
        if(e->wheel.y > 0)
        {
            if (engine->zoom < 1)
                engine->zoom *= 2;
            else
                engine->zoom += 1;
            SetScale(engine);
        }
        else if(e->wheel.y < 0)
        {
            if (engine->zoom <= 1)
                engine->zoom /= 2;
            else
                engine->zoom -= 1;
            SetScale(engine);
        }
    }
}

void EngineUpdateCamera(Engine* engine, float x, float y, int width, int height)
{
    SDL_FRect* camera = &engine->camera;

    camera->x = x + (width / 2) - (camera->w / 2);
    camera->y = y + (height / 2) - (camera->h / 2);
    if (camera->x < 0)
        camera->x = 0;
    else if (camera->x > engine->levelWidth - camera->w)
        camera->x = engine->levelWidth - camera->w;

    if (camera->y < 0)
        camera->y = 0;
    else if (camera->y > engine->levelHeight - camera->h)
        camera->y = engine->levelHeight - camera->h;
}

void EngineGetScreenRect(Engine* engine, SDL_Rect* rect, float x, float y, int w, int h)
{
    rect->x = engine->renderStartPoint.x + ((x - engine->camera.x) * engine->zoom);
    rect->y = engine->renderStartPoint.y + ((y - engine->camera.y) * engine->zoom);
    rect->w = w * engine->zoom;
    rect->h = h * engine->zoom;
}

void EngineUpdate(Engine* engine)
{
    Uint64 ticks = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    engine->deltaTime = (double)(ticks - engine->lastTicks) / freq;
    engine->lastTicks = ticks;
    engine->time = (double)engine->lastTicks / freq;

    int fps = 1.0 / engine->deltaTime;
    engine->fpses[engine->fpsIndex] = fps;
    engine->fpsIndex = (engine->fpsIndex + 1) % FPSES_COUNT;

    engine->fps = 0;
    for (int i = 0; i < FPSES_COUNT; i++)
        engine->fps += engine->fpses[i];
    engine->fps /= FPSES_COUNT;
}

void RenderText(Engine* engine, const char* text, SDL_Color color, int x, int y)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(engine->font, text, color);
    if(textSurface == NULL)
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(engine->renderer, textSurface);
    if(texture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        engine->textRect.x = x;
        engine->textRect.y = y;
        engine->textRect.w = textSurface->w;
        engine->textRect.h = textSurface->h;

        SDL_RenderCopy(engine->renderer, texture, NULL, &engine->textRect);
    }

    if (texture != NULL)
        SDL_DestroyTexture(texture);
    SDL_FreeSurface(textSurface);
}