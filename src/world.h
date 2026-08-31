/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL3/SDL.h>

#include "vectors.h"
#include "animation.h"

extern SDL_Renderer* renderer;

typedef enum {
	player,
	enemy,
	element,
	interactable,
	weapon
}type;

typedef struct {
	SDL_Texture* spritesheet;
	Transform* transform;
	animation* animations;
	uint8_t* hp;
	uint8_t* power;
	uint8_t* defense;
	uint8_t* mass;
	uint8_t* speed;
	uint8_t* cooldown;
	uint8_t* wallCling;
	uint8_t* animationPlaying;
	bool* flags;
	bool* anchored;
	bool* grounded;
	bool* canCollide;
	bool active;
	type type;
}entity;

typedef struct {
	entity entities[64];
}world;

extern Box camera;
extern Box deadZone;
extern Box scrollStop;

extern uint8_t worldIndex;
extern world World;

extern bool quit;

void initPlayer(sprite sprite, const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed, uint8_t wallCling);
void newEnemy(sprite sprite, const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed);
void newElement(sprite sprite, const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t mass, bool canCollide, bool anchored);
void cleanData();
