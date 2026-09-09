/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/

#include <cstdint>

#include "functions.hpp"

extern "C" {
	#include "world.h"
}

void newEnemyFunction(uint8_t Enemy, bool chasePlayer, uint8_t detectDistance) {
	if (chasePlayer) {
		*functions[Enemy].function = []() {
			if (
				World.entities[Enemy].transform->x < World.entities[0].transform->x + detectDistance &&
				World.entities[Enemy].transform->x + detectDistance > World.entities[0].transform->x
			) {
				if (World.entities[0].transform->position.x < World.entities[Enemy].transform->position.x) {
					World.entities[Enemy].transform->velocity.x -= *World.entities[Enemy].speed
				} else if (World.entities[0].transform->position.x > World.entities[Enemy].transform->position.x) {
					World.entities[Enemy].transform->velocity.x += *World.entities[Enemy].speed
				}
			}
		};
	}
}
