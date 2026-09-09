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

function functions[64];

void runFunctions() {
	for (uint8_t i = 0; i < 64; i++) {
		if (*World.entities[i].runFunction && *World.entities[i].triggered) {
			functions[i]->function();
		}
	}
}
void newFunction() {
	//functions[worldIndex].function =
}
