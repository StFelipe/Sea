#ifndef EDITOR_H
#define EDITOR_H

#include "engine.h"

typedef enum EditorSelection {
    EditorSave = 0,
    EditorLoad = 1,
    EditorBack = 2
} EditorSelection;

typedef struct Editor {
} Editor;

void InitEditor(Editor* editor, Engine* engine);
void FreeEditor(Editor* editor);

void EditorHandleEvent(Editor* editor, Engine* engine, SDL_Event* e);
void UpdateEditor(Editor* editor, Engine* engine);

#endif