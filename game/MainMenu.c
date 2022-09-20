#include "MainMenu.h"


void InitMainMenu(MainMenu* mainMenu, Engine* engine)
{
    mainMenu->uiOption.selection = MainMenuPlay;
    mainMenu->uiOption.normalTextColor = NORMAL_TEXT_COLOR;
    mainMenu->uiOption.selectedTextColor = SELECTED_TEXT_COLOR;
}

void FreeMainMenu(MainMenu* mainMenu)
{

}


void MainMenuHandleEvent(MainMenu* mainMenu, Engine* engine, SDL_Event* e)
{
}

void RenderTextInMiddle(Engine* engine, const char* text, SDL_Color color, int yOffset)
{
    SDL_Rect textRect;
    SDL_Texture* textTexture = GetTextTexture(engine, text, color, 0, 0, &textRect);
    textRect.x = 100;
    textRect.y = (engine->windowHeight / 4) + yOffset;

    RenderTextFromTexture(engine, textTexture, &textRect);
}
bool UpdateMainMenu(MainMenu* mainMenu, Engine* engine)
{
    if (IsKeyPresed(engine->input, SDL_SCANCODE_RETURN))
    {
        if (mainMenu->uiOption.selection == MainMenuPlay)
            engine->state = GameState;
        if (mainMenu->uiOption.selection == MainMenuEditor)
            engine->state = EditorState;
        if (mainMenu->uiOption.selection == MainMenuQuit)
            return true;
    }

    UiCheckUpDown(&mainMenu->uiOption, engine, MainMenuQuit);


    SDL_RenderClear(engine->renderer);

    mainMenu->drawRect.x = 0;
    mainMenu->drawRect.x = 0;
    mainMenu->drawRect.w = engine->windowWidth;
    mainMenu->drawRect.h = engine->windowHeight;
    SDL_SetRenderDrawColor(engine->renderer, 30, 30, 90, 150);
    SDL_RenderFillRect(engine->renderer, &mainMenu->drawRect);

    RenderTextInMiddle(engine, "Play", GetColorOnSelection(&mainMenu->uiOption, MainMenuPlay), 0);
    RenderTextInMiddle(engine, "Editor", GetColorOnSelection(&mainMenu->uiOption, MainMenuEditor), 50);
    RenderTextInMiddle(engine, "Quit", GetColorOnSelection(&mainMenu->uiOption, MainMenuQuit), 100);

    SDL_RenderPresent(engine->renderer);

    return false;
}