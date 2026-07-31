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

void loadAnimation(sprite sprite, animation* animation, const char* path);
void freeAnimation(animation* animation);
void playAnimations(SDL_Renderer* renderer, SDL_Texture* spritesheet);
