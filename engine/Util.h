#ifndef UTIL_H
#define UTIL_H

#include <SDL.h>
#include <stdbool.h>

typedef struct Anim {
    int w;
    int h;

    int frames;
    int frame;
    int dir;
    bool isAnimating;

    int defaultFrame;
    float speed;
    double timer;

    int up;
    int down;
    int left;
    int right;
} Anim;

void SetAnimRect(Anim* anim, SDL_Rect* rect);

float GetPointSqrMagnitude(const SDL_FPoint* point);
float GetPointMagnitude(const SDL_FPoint* point);
void NormalizePoint(SDL_FPoint* point);
void AddPoint(SDL_FPoint* pointA, const SDL_FPoint* pointB);
void SubPoint(SDL_FPoint* pointA, const SDL_FPoint* pointB);
void MulPoint(SDL_FPoint* pointA, float factor);

#endif