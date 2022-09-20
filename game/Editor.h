#ifndef EDITOR_H
#define EDITOR_H

#include "engine.h"

typedef struct Editor {

} Editor;

void InitEditor(Editor* editor, Engine* engine);
void FreeEditor(Editor* editor);

void EditorHandleEvent(Editor* editor, Engine* engine, SDL_Event* e);
void UpdateEditor(Editor* editor, Engine* engine);

#endif