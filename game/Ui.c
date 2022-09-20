#include "Ui.h"

void UiCheckUpDown(UiOption* option, Engine* engine, int maxSelection)
{
    if (IsKeyPresed(engine->input, SDL_SCANCODE_UP))
    {
        option->selection--;
        if ((int)(option->selection) < 0)
            option->selection = maxSelection;
    }
    else if (IsKeyPresed(engine->input, SDL_SCANCODE_DOWN))
    {
        option->selection = (option->selection + 1) % (maxSelection + 1);
    }
}

SDL_Color GetColorOnSelection(UiOption* option, int selection)
{
    return (option->selection == selection ? option->selectedTextColor : option->normalTextColor);
}
