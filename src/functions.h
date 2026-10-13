/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

typedef struct {
	SDL_Renderer* renderer;
	sprite sprite;
	animation* animation;
	const char* spritePath;
	const char* dataPath;
	bool called;
}Animation_call_wrapper;

typedef struct {
	const char* spritePath;
	const char* animationPath;
	float x;
	float y;
	float width;
	float height;
	uint8_t mass;
	bool canCollide;
	bool anchored;
	const char* script;
	bool called;
}Element_call_wrapper;

typedef struct {
	const char* spritePath;
	const char* animationPath;
	float x;
	float y;
	float width;
	float height;
	uint8_t power;
	uint8_t defense;
	uint8_t mass;
	uint8_t speed;
	const char* script;
	bool called;
}Enemy_call_wrapper;

typedef struct {
	const char* dataPath;
	bool called;
}Map_call_wrapper;

typedef struct {
	const char* save;
	bool called;
}Save_call_wrapper;

extern Animation_call_wrapper animation_call_wrapper;
extern Element_call_wrapper element_call_wrapper;
extern Enemy_call_wrapper enemy_call_wrapper;
extern Map_call_wrapper map_call_wrapper;
extern Save_call_wrapper save_call_wrapper;

void* runFunctions(void* arg);
void registerFunctions();
