#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "vectors.h"
#include "animation.h"

typedef enum {
	player,
	enemy,
	element,
	interactable,
	weapon;
}type;

typedef struct {
	Transform* transform;
	animation* animations;
	Vec* knockback;
	uint8_t* hp;
	uint8_t* power;
	uint8_t* defense;
	uint8_t* mass;
	uint8_t* speed;
	uint8_t* cooldown;
	bool* flags;
	bool* anchored;
	bool* grounded;
	bool active;
	type type;
}entity;

typedef struct {
	entity entities[64];
}world;

extern uint8_t worldIndex;


void initPlayer(sprite sprite, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed) {
	World.entities[0].transform = (Transform*)malloc(sizeof(Transform));
	World.entities[0].transform->position = {x, y};
	World.entities[0].transform->scale = (Vec) {width, height};
	World.entities[0].transform->velocity = (Vec) {0.0f, 0.0f};

	*World.entities[0].hp = 100;
	*World.entities[0].power = power;
	*World.entities[0].defense = defense;
	*World.entities[0].mass = mass;
	*World.entities[0].speed = speed;

	loadAnimation(sprite, World.entities[0].animation, animationPath);

	*World.entities[0].active = true;
	worldIndex++;
}

void newEnemy(sprite sprite, const char* animationPath, float x, float y, float width, float height, uint8_t power, uint8_t defense, uint8_t mass, uint8_t speed) {
	World.entities[worldIndex].Transform->position = (Vec) {x, y};
	World.entities[worldIndex].Transform->scale = (Vec) {width, height};
	World.entities[worldIndex].Transform->velocity = {0.0, 0.0};

	*World.entities[worldIndex].hp = 100;
	*World.entities[worldIndex].power = power;
	*World.entities[worldIndex].defense = defense;
	*World.entities[worldIndex].mass = mass;
	*World.entities[worldIndex].speed = speed;
	

	loadAnimation(sprite, World.entities[worldIndex].animation, animationPath);

	*World.entities[worldIndex].active = true;
	worldIndex++;
}

void newElement(sprite sprite, const char* animationPath, float x, float y, float width, float height, bool canCollide) {
	World.entities[worldIndex].Transform->position = (Vec) {x, y};
	World.entities[worldIndex].Transform->scale = (Vec) {width, height};
	World.entities[worldIndex].Transform->velocity = {0.0f, 0.0f};

	*World.entities[worldIndex].canCollide = canCollide;
	
	loadAnimation(sprite sprite, World.entities[worldIndex].animation, animationPath);

	*World.entities[worldIndex].active = true;
	worldIndex++;
}

void cleanData() {
	for (uint8_t i = 0; i < worldIndex; i++) {
		if (!World.entities[i].active) {
			free(World.entities[i].transform);
			free(World.entities[i]animations);
			knockback;
			hp;
			power;
			defense;
			mass;
			speed;
			cooldown;
			flags;
			anchored;
			grounded;
		}
	}
}

extern world World;
