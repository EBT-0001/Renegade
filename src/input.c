#include <stdbool.h>
#include <SDL3/SDL.h>

#include "world.h"
#include "physics.h"

void processInput(SDL_Event* eventHandler, bool* quit) {
	while (SDL_PollEvent(eventHandler)) {
		if (eventHandler->type == SDL_EVENT_QUIT) {
			*quit = true;
		}
	}

	const bool* key_states = SDL_GetKeyboardState(NULL);
	
	if (key_states[SDL_SCANCODE_W]) {
	}
	if (key_states[SDL_SCANCODE_A]) {
		World.Player.transform.velocity.x = -World.Player.speed;
	}
	if (key_states[SDL_SCANCODE_S]) {
	}
	if (key_states[SDL_SCANCODE_D]) {
		World.Player.transform.velocity.x = World.Player.speed;
	}
	if (key_states[SDL_SCANCODE_LSHIFT] && (key_states[SDL_SCANCODE_A] || key_states[SDL_SCANCODE_D])) {
		World.Player.transform.velocity.x *= 2.0f;
	}
	if (key_states[SDL_SCANCODE_SPACE] && World.Player.grounded) {
		World.Player.transform.velocity.y -= (500.0f - World.Player.mass);
		World.Player.grounded = false;
	}
	if (key_states[SDL_SCANCODE_LCTRL] && key_states[SDL_SCANCODE_ESCAPE]) {
		*quit = true;
	}
}
