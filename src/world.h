#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "vectors.h"
#include "animation.h"

typedef struct {
	animation animations[9];
	Transform transform;
	Vec knockback;
	uint8_t animationPlaying;
	uint8_t hp;
	uint8_t defense;
	uint8_t speed;
	uint8_t power;
	uint8_t mass;
	uint8_t cooldown;
	bool grounded;
	bool active;
}player;

typedef struct {
	animation animations[16];
	Transform transform;
	Vec knockback;
	uint8_t animationPlaying;
	uint8_t hp;
	uint8_t defense;
	uint8_t speed;
	uint8_t power;
	uint8_t mass;
	uint8_t cooldown;
	bool canCollide;
	bool active;
}enemy;

typedef struct {
	animation animations[3];
	Transform transform;
	uint8_t animationPlaying;
	bool canCollide;
	bool active;
}interactable;

typedef struct {
	animation animation;
	Transform transform;
	uint8_t mass;
	bool anchored;
	bool canCollide;
	bool active;
}element;

typedef struct {
	element elements[32];
	enemy enemies[16];
	player Player;
	interactable interactables[8];
}world;

extern world World;
