#pragma once

#include <stdint.h>
#include <SDL3/SDL.h>

#include "vectors.h"

typedef enum {
	playerIdle = 0
}sprite;

typedef struct {
	Vec texCoords;
	Vec texDimensions;
}texture;
typedef struct {
	texture* frames;
	uint8_t frameCount;
	uint8_t frameClock;
}animation;

void animationConfig(sprite sprite, uint8_t frameCount, animation* animation);
void animationRemove(animation* animation);
void playAnimations(SDL_Renderer* renderer, SDL_Texture* spritesheet);
