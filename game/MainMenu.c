#include "MainMenu.h"


void InitMainMenu(MainMenu* mainMenu, Engine* engine)
{
    mainMenu->selection = MainMenuPlay;
    mainMenu->normalTextColor = (SDL_Color){ 100, 100, 100, 255 };
    mainMenu->selectedTextColor = (SDL_Color){ 0, 0, 0, 255 };
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
        if (mainMenu->selection == MainMenuPlay)
            engine->state = GameState;
        if (mainMenu->selection == MainMenuEditor)
            engine->state = EditorState;
        if (mainMenu->selection == MainMenuQuit)
            return true;
    }

    if (IsKeyPresed(engine->input, SDL_SCANCODE_UP))
    {
        mainMenu->selection--;
        if ((int)(mainMenu->selection) < 0)
            mainMenu->selection = MainMenuQuit;
    }
    else if (IsKeyPresed(engine->input, SDL_SCANCODE_DOWN))
    {
        mainMenu->selection = (mainMenu->selection + 1) % (MainMenuQuit + 1);
    }


    SDL_RenderClear(engine->renderer);

    RenderTextInMiddle(engine, "Play", (mainMenu->selection == MainMenuPlay ? mainMenu->selectedTextColor : mainMenu->normalTextColor), 0);
    RenderTextInMiddle(engine, "Editor", (mainMenu->selection == MainMenuEditor ? mainMenu->selectedTextColor : mainMenu->normalTextColor), 50);
    RenderTextInMiddle(engine, "Quit", (mainMenu->selection == MainMenuQuit ? mainMenu->selectedTextColor : mainMenu->normalTextColor), 100);

    SDL_RenderPresent(engine->renderer);

    return false;
}