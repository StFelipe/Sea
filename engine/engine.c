#include "engine.h"
#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <stdbool.h>

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

int InitEngine(const char* windowName, int windowWidth, int windowHeight, Engine* engine)
{
    engine->windowWidth = windowWidth;
    engine->windowHeight = windowHeight;

    engine->lastTicks = 0;
    engine->deltaTime = 0;
    engine->fps = 0;
    engine->fpsIndex = 0;

    engine->isPaused = false;

    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
        engine->input[i] = KeyStateNone;

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

void EngineHandleEvent(Engine* engine, const SDL_Event* e)
{
    if (e->type == SDL_WINDOWEVENT)
    {
        if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            engine->windowWidth = e->window.data1;
            engine->windowHeight = e->window.data2;
        }
    }
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

    if (engine->isPaused)
        engine->deltaTime = 0;
}

SDL_Texture* GetTextTexture(Engine* engine, const char* text, SDL_Color color, int x, int y, SDL_Rect* textureRect)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(engine->font, text, color);
    if(textSurface == NULL)
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(engine->renderer, textSurface);
    if(texture == NULL)
    {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
    }

    textureRect->x = x;
    textureRect->y = y;
    textureRect->w = textSurface->w;
    textureRect->h = textSurface->h;
    
    SDL_FreeSurface(textSurface);
    return texture;
}

void RenderText(Engine* engine, const char* text, SDL_Color color, int x, int y)
{
    SDL_Texture* texture = GetTextTexture(engine, text, color, x, y, &engine->textRect);
    RenderTextFromTexture(engine, texture, &engine->textRect);
}

void RenderTextFromTexture(Engine* engine, SDL_Texture* texture, SDL_Rect* rect)
{
    SDL_RenderCopy(engine->renderer, texture, NULL, rect);
    if (texture != NULL)
        SDL_DestroyTexture(texture);
}

bool CheckKeyFlag(int* input, SDL_Scancode key, KeyState keyFlag)
{
    return (input[key] & (int)keyFlag) == (int)keyFlag;
}
void SetKeyFlag(int* input, SDL_Scancode key, KeyState keyFlag)
{
    input[key] |= (int)keyFlag;
}
void RemoveKeyFlag(int* input, SDL_Scancode key, KeyState keyFlag)
{
    input[key] &= ~(int)keyFlag;
}
void EngineUpdateInput(Engine* engine)
{
    int num;
    const Uint8* keyStates = SDL_GetKeyboardState(&num);

    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        RemoveKeyFlag(engine->input, i, KeyReleased | KeyPressed);

        if (keyStates[i])
        {
            if (!CheckKeyFlag(engine->input, i, KeyDown))
                SetKeyFlag(engine->input, i, KeyPressed);
            SetKeyFlag(engine->input, i, KeyDown);
        }
        else if (CheckKeyFlag(engine->input, i, KeyDown))
        {
            engine->input[i] = KeyReleased;
        }
    }
}

bool IsKeyDown(int* input, SDL_Scancode key)
{
    return CheckKeyFlag(input, key, KeyDown);
}
bool IsKeyPresed(int* input, SDL_Scancode key)
{
    return CheckKeyFlag(input, key, KeyPressed);
}
bool IsKeyReleased(int* input, SDL_Scancode key)
{
    return CheckKeyFlag(input, key, KeyReleased);
}