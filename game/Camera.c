#include "Camera.h"
#include "engine.h"
#include <SDL.h>

void CameraInit(Camera* camera, Engine* engine)
{
    camera->renderStartPoint = (SDL_Point){ 0, 0 };
    camera->rect = (SDL_FRect){ 0, 0, engine->windowWidth / STARTING_ZOOM, engine->windowHeight / STARTING_ZOOM };
    camera->zoom = STARTING_ZOOM;
}

void SetCameraScale(Camera* camera, Engine* engine, int levelWidth, int levelHeight)
{
    int realLevelWidth = levelWidth * camera->zoom;
    int realLevelHeight = levelHeight * camera->zoom;

    if (realLevelWidth <= engine->windowWidth)
        camera->rect.w = realLevelWidth;
    else
        camera->rect.w = engine->windowWidth / camera->zoom;

    if (realLevelHeight <= engine->windowHeight)
        camera->rect.h = realLevelHeight;
    else
        camera->rect.h = engine->windowHeight / camera->zoom;


    camera->renderStartPoint.x = (engine->windowWidth / 2) - (realLevelWidth / 2);
    camera->renderStartPoint.y = (engine->windowHeight / 2) - (realLevelHeight / 2);
    if (camera->renderStartPoint.x < 0)
        camera->renderStartPoint.x = 0;
    if (camera->renderStartPoint.y < 0)
        camera->renderStartPoint.y = 0;
}

void UpdateCamera(Camera* camera, float x, float y, int width, int height, int levelWidth, int levelHeight)
{
    SDL_FRect* cameraRect = &camera->rect;

    cameraRect->x = x + (width / 2) - (cameraRect->w / 2);
    cameraRect->y = y + (height / 2) - (cameraRect->h / 2);
    if (cameraRect->x < 0)
        cameraRect->x = 0;
    else if (cameraRect->x > levelWidth - cameraRect->w)
        cameraRect->x = levelWidth - cameraRect->w;

    if (cameraRect->y < 0)
        cameraRect->y = 0;
    else if (cameraRect->y > levelHeight - cameraRect->h)
        cameraRect->y = levelHeight - cameraRect->h;
}

void GetScreenRectFromCamera(Camera* camera, SDL_Rect* rect, float x, float y, int w, int h)
{
    rect->x = camera->renderStartPoint.x + ((x - camera->rect.x) * camera->zoom);
    rect->y = camera->renderStartPoint.y + ((y - camera->rect.y) * camera->zoom);
    rect->w = w * camera->zoom;
    rect->h = h * camera->zoom;
}

void CameraZoomIn(Camera* camera, Engine* engine, int levelWidth, int levelHeight)
{
    if (camera->zoom < 1)
        camera->zoom *= 2;
    else
        camera->zoom += 1;
    SetCameraScale(camera, engine, levelWidth, levelHeight);
}

void CameraZoomOut(Camera* camera, Engine* engine, int levelWidth, int levelHeight)
{
    if (camera->zoom <= 1)
        camera->zoom /= 2;
    else
        camera->zoom -= 1;
    SetCameraScale(camera, engine, levelWidth, levelHeight);
}