#ifndef UI_H
#define UI_H

#include <SDL.h>
#include "engine.h"

#define SELECTED_TEXT_COLOR (SDL_Color){ 237, 185, 12, 255 }
#define NORMAL_TEXT_COLOR (SDL_Color){ 138, 111, 23, 255 }

typedef struct UiOption {
    int selection;
    SDL_Color selectedTextColor;
    SDL_Color normalTextColor;
} UiOption;

void UiCheckUpDown(UiOption* option, Engine* engine, int maxSelection);
SDL_Color GetColorOnSelection(UiOption* option, int selection); 

#endif