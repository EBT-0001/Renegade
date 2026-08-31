/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#include <stdbool.h>
#include <SDL3/SDL.h>

#include "world.h"
#include "physics.h"

void processInput(void* arg) {
	SDL_Event* eventHandler = (SDL_Event*) arg;
	while (!quit) {
		while (SDL_PollEvent(eventHandler)) {
			if (eventHandler->type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}

		const bool* key_states = SDL_GetKeyboardState(NULL);

		if (key_states[SDL_SCANCODE_W]) {
		}
		if (key_states[SDL_SCANCODE_A]) {
			World.entities[0].transform->velocity.x = -*World.entities[0].speed;
		}
		if (key_states[SDL_SCANCODE_S]) {
		}
		if (key_states[SDL_SCANCODE_D]) {
			World.entities[0].transform->velocity.x = *World.entities[0].speed;
		}
		if (key_states[SDL_SCANCODE_LSHIFT] && (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_D])) {
			World.entities[0].transform->velocity.x *= 2.0f;
		}
		if (key_states[SDL_SCANCODE_SPACE] && *World.entities[0].grounded) {
			World.entities[0].transform->velocity.y -= (500.0f - *World.entities[0].mass);
			*World.entities[0].grounded = false;
		}
	}
}
