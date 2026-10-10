/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/

#include <stdlib.h>
#include <stdint.h>

#include "world.h"
#include "vectors.h"

void initPlayer(const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed, uint8_t wallCling, const char* script) {
	World.entities[0].transform = (Transform*) malloc(sizeof(Transform));

	World.entities[0].transform->position = (Vec) {x, y};
	World.entities[0].transform->scale = (Vec) {width, height};
	World.entities[0].transform->velocity = (Vec) {0.0f, 0.0f};

	World.entities[0].hp = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].power = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].defense = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].mass = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].speed = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].wallCling = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[0].cooldown = (uint8_t*) calloc(1, sizeof(uint8_t));
	World.entities[0].animationPlaying = (uint8_t*) calloc(1, sizeof(uint8_t));

	*World.entities[0].hp = 100;
	*World.entities[0].power = power;
	*World.entities[0].defense = defense;
	*World.entities[0].mass = mass;
	*World.entities[0].speed = speed;
	*World.entities[0].wallCling = wallCling;

	World.entities[0].anchored = (bool*) malloc(sizeof(bool));
	World.entities[0].grounded = (bool*) malloc(sizeof(bool));
	World.entities[0].canCollide = (bool*) malloc(sizeof(bool));

	*World.entities[0].anchored = false;
	*World.entities[0].grounded = false;
	*World.entities[0].canCollide = false;

	World.entities[0].animations = (animation*) malloc(8 * sizeof(animation));

	World.entities[0].spritesheet = loadAnimation(renderer, Idle, &World.entities[0].animations[0], spritePath, animationPath);

	World.entities[0].script = script;

	World.entities[0].active = true;
}

void newEnemy(const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed, const char* script) {
	World.entities[worldIndex].transform = (Transform*) malloc(sizeof(Transform));

	World.entities[worldIndex].transform->position = (Vec) {x, y};
	World.entities[worldIndex].transform->scale = (Vec) {width, height};
	World.entities[worldIndex].transform->velocity = (Vec) {0.0f, 0.0f};

	World.entities[worldIndex].hp = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].power = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].defense = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].mass = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].speed = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].cooldown = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].animationPlaying = (uint8_t*) malloc(sizeof(uint8_t));

	*World.entities[worldIndex].hp = 100;
	*World.entities[worldIndex].power = power;
	*World.entities[worldIndex].defense = defense;
	*World.entities[worldIndex].mass = mass;
	*World.entities[worldIndex].speed = speed;
	*World.entities[worldIndex].cooldown = 0;
	*World.entities[worldIndex].animationPlaying = 0;

	World.entities[worldIndex].animations = (animation*) malloc(8 * (sizeof(animation)));

	World.entities[worldIndex].spritesheet = loadAnimation(renderer, Idle, &World.entities[worldIndex].animations[0], spritePath, animationPath);

	World.entities[0].script = script;

	World.entities[worldIndex].active = true;
}

void newElement(const char* spritePath, const char* animationPath, float x, float y, float width, float height, uint8_t mass, bool canCollide, bool anchored, const char* script) {
	World.entities[worldIndex].transform = (Transform*) malloc(sizeof(Transform));

	World.entities[worldIndex].transform->position = (Vec) {x, y};
	World.entities[worldIndex].transform->scale = (Vec) {width, height};
	World.entities[worldIndex].transform->velocity = (Vec) {0.0f, 0.0f};

	World.entities[worldIndex].mass = (uint8_t*) malloc(sizeof(uint8_t));
	World.entities[worldIndex].animationPlaying = (uint8_t*) malloc(sizeof(uint8_t));

	*World.entities[worldIndex].mass = mass;
	*World.entities[worldIndex].animationPlaying = 0;

	World.entities[worldIndex].canCollide = (bool*) malloc(sizeof(bool));
	World.entities[worldIndex].anchored = (bool*) malloc(sizeof(bool));

	*World.entities[worldIndex].canCollide = canCollide;
	*World.entities[worldIndex].anchored = anchored;

	World.entities[worldIndex].animations = (animation*) malloc(4 * sizeof(animation));

	World.entities[worldIndex].spritesheet = loadAnimation(renderer, Idle, &World.entities[worldIndex].animations[0], spritePath, animationPath);

	World.entities[worldIndex].script = script;

	World.entities[worldIndex].active = true;
	worldIndex++;
}

void cleanData() {
	for (uint8_t i = 0; i < worldIndex; i++) {
		if (!World.entities[i].active) {
			if (World.entities[i].transform != NULL) {free(World.entities[i].transform); World.entities[i].transform = NULL;}
			if (World.entities[i].animations != NULL) {free(World.entities[i].animations); World.entities[i].animations = NULL;}
			if (World.entities[i].hp != NULL) {free(World.entities[i].hp); World.entities[i].hp = NULL;}
			if (World.entities[i].power != NULL) {free(World.entities[i].power); World.entities[i].power = NULL;}
			if (World.entities[i].defense != NULL) {free(World.entities[i].defense); World.entities[i].defense = NULL;}
			if (World.entities[i].mass != NULL) {free(World.entities[i].mass); World.entities[i].mass = NULL;}
			if (World.entities[i].speed != NULL) {free(World.entities[i].speed); World.entities[i].speed = NULL;}
			if (World.entities[i].cooldown != NULL) {free(World.entities[i].cooldown); World.entities[i].cooldown = NULL;}
			if (World.entities[i].flags != NULL) {free(World.entities[i].flags); World.entities[i].flags = NULL;}
			if (World.entities[i].anchored != NULL) {free(World.entities[i].anchored); World.entities[i].anchored = NULL;}
			if (World.entities[i].grounded != NULL) {free(World.entities[i].grounded); World.entities[i].grounded = NULL;}
			if (World.entities[i].spritesheet != NULL) {SDL_DestroyTexture(World.entities[i].spritesheet); World.entities[i].spritesheet = NULL;}
			if (i != 64) {
				if (World.entities[i + 1].active && i) {
					World.entities[i].transform = World.entities[i + 1].transform;
					World.entities[i + 1].transform = NULL;
					World.entities[i].animations = World.entities[i + 1].animations;
					World.entities[i + 1].animations = NULL;
					World.entities[i].hp = World.entities[i + 1].hp;
					World.entities[i + 1].hp = NULL;
					World.entities[i].power = World.entities[i + 1].power;
					World.entities[i + 1].power = NULL;
					World.entities[i].defense = World.entities[i + 1].defense;
					World.entities[i + 1].defense = NULL;
					World.entities[i].mass = World.entities[i + 1].mass;
					World.entities[i + 1].mass = NULL;
					World.entities[i].speed = World.entities[i + 1].speed;
					World.entities[i + 1].speed = NULL;
					World.entities[i].cooldown = World.entities[i + 1].cooldown;
					World.entities[i + 1].cooldown = NULL;
					World.entities[i].flags = World.entities[i + 1].flags;
					World.entities[i + 1].flags = NULL;
					World.entities[i].anchored = World.entities[i + 1].anchored;
					World.entities[i + 1].anchored = NULL;
					World.entities[i].grounded = World.entities[i + 1].grounded;
					World.entities[i + 1].grounded = NULL;
					World.entities[i].spritesheet = World.entities[i + 1].spritesheet;
					World.entities[i + 1].spritesheet = NULL;

					World.entities[i].active = true;
					World.entities[i + 1].active = false;
					if (i + 1 == worldIndex) worldIndex--;
				}
			}
		}
	}
}
