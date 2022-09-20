#ifndef CAMERA_H
#define CAMERA_H

#include <SDL.h>
#include "engine.h"

#define STARTING_ZOOM 4

typedef struct Camera
{
    SDL_Point renderStartPoint;
    SDL_FRect rect;
    float zoom;
} Camera;

void CameraInit(Camera* camera, Engine* engine);

void SetCameraScale(Camera* camera, Engine* engine, int levelWidth, int levelHeight);
void UpdateCamera(Camera* camera, float x, float y, int width, int height, int levelWidth, int levelHeight);
void GetScreenRectFromCamera(Camera* camera, SDL_Rect* rect, float x, float y, int w, int h);
void CameraZoomIn(Camera* camera, Engine* engine, int levelWidth, int levelHeight);
void CameraZoomOut(Camera* camera, Engine* engine, int levelWidth, int levelHeight);

#endif