#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "engine.h"

typedef struct MainMenu {

} MainMenu;

void InitMainMenu(MainMenu* mainMenu, Engine* engine);
void FreeMainMenu(MainMenu* mainMenu);

void MainMenuHandleEvent(MainMenu* mainMenu, Engine* engine, SDL_Event* e);
void UpdateMainMenu(MainMenu* mainMenu, Engine* engine, const Uint8* keyStates);

#endif