#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>

#include "vectors.h"

typedef enum {
	Idle = 0
}sprite;

typedef struct {
	Vec texCoords;
	Vec texDimensions;
}texture;
typedef struct {
	texture* frames;
	uint8_t frameCount;
	uint8_t frameClock;
	uint8_t animationPlaying;
}animation;

SDL_Texture* loadAnimation(SDL_Renderer* renderer, sprite sprite, animation* animation, const char* spritePath, const char* dataPath);
void freeAnimation(animation* animation);
void* playAnimations(SDL_Renderer* renderer);
