#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define FPSES_COUNT 100

typedef struct Engine
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    TTF_Font* font;
    SDL_Rect textRect;

    int windowWidth;
    int windowHeight;
    int levelWidth;
    int levelHeight;

    SDL_Point renderStartPoint;
    SDL_FRect camera;
    float zoom;

    Uint64 lastTicks;
    double deltaTime;
    double time;
    int fpses[FPSES_COUNT];
    int fpsIndex;
    int fps;
} Engine;

void InitManagers(Engine* engine);
void FreeManagers(Engine* engine);

int InitEngine(const char* windowName, int windowWidth, int windowHeight, int startingZoom, Engine* engine);
void CloseEngine();

SDL_Texture* LoadTexture(char* path, SDL_Renderer* renderer);

void EngineHandleEvent(Engine* engine, const SDL_Event* e);
void EngineUpdateCamera(Engine* engine, float x, float y, int width, int height);
void EngineGetScreenRect(Engine* engine, SDL_Rect* rect, float x, float y, int w, int h);
void EngineUpdate(Engine* engine);
void RenderText(Engine* engine, const char* text, SDL_Color color, int x, int y);

#endif