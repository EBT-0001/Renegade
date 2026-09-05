/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#include <cstdint>

extern "C" {
	#include "world.h"
}

typedef struct {
	void (*function)() = nullptr;
	bool assigned;
} enemyLogic;

auto newEnemyLogic(bool chasePlayer, uint8_t detectDistance) {
	if (chasePlayer) {
		
	}
	return []() {
		
	};
}
