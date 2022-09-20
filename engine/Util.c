#include "Util.h"
#include <SDL.h>
#include <math.h>

void SetAnimRect(Anim* anim, SDL_Rect* rect)
{
    rect->x = anim->frame * anim->w;
    rect->y = anim->dir * anim->h;
    rect->w = anim->w;
    rect->h = anim->h;
}


float GetPointSqrMagnitude(const SDL_FPoint* point)
{
    return (point->x * point->x) + (point->y * point->y);
}
float GetPointMagnitude(const SDL_FPoint* point)
{
    return sqrt(GetPointSqrMagnitude(point));
}

void NormalizePoint(SDL_FPoint* point)
{
    double l = GetPointMagnitude(point);
    point->x /= l;
    point->y /= l;
}

void AddPoint(SDL_FPoint* pointA, const SDL_FPoint* pointB)
{
    pointA->x += pointB->x;
    pointA->y += pointB->y;
}

void SubPoint(SDL_FPoint* pointA, const SDL_FPoint* pointB)
{
    pointA->x -= pointB->x;
    pointA->y -= pointB->y;
}

void MulPoint(SDL_FPoint* pointA, float factor)
{
    pointA->x *= factor;
    pointA->y *= factor;
}