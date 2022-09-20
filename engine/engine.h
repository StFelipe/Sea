#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#define FPSES_COUNT 100

typedef enum State {
    MainMenuState,
    GameState,
    EditorState
} State;

typedef enum KeyState {
    KeyStateNone = 0,
    KeyDown = 1 << 0,
    KeyPressed = 1 << 1,
    KeyReleased = 1 << 2
} KeyState;

typedef struct Engine
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    State state;

    TTF_Font* font;
    SDL_Rect textRect;

    int windowWidth;
    int windowHeight;

    Uint64 lastTicks;
    double deltaTime;
    double time;
    int fpses[FPSES_COUNT];
    int fpsIndex;
    int fps;

    int input[SDL_NUM_SCANCODES];
} Engine;

void InitManagers(Engine* engine);
void FreeManagers(Engine* engine);

int InitEngine(const char* windowName, int windowWidth, int windowHeight, Engine* engine);
void CloseEngine();

SDL_Texture* LoadTexture(char* path, SDL_Renderer* renderer);

void EngineUpdate(Engine* engine);
void EngineHandleEvent(Engine* engine, const SDL_Event* e);
void EngineUpdateInput(Engine* engine);

SDL_Texture* GetTextTexture(Engine* engine, const char* text, SDL_Color color, int x, int y, SDL_Rect* textureRect);
void RenderText(Engine* engine, const char* text, SDL_Color color, int x, int y);
void RenderTextFromTexture(Engine* engine, SDL_Texture* texture, SDL_Rect* rect);

bool IsKeyDown(int* input, SDL_Scancode key);
bool IsKeyPresed(int* input, SDL_Scancode key);
bool IsKeyReleased(int* input, SDL_Scancode key);


#endif