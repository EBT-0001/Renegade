#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "world.h"
#include "physics.h"
#include "animation.h"
#include "input.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* spritesheet = NULL;
SDL_Texture* background = NULL;

Vec camPos = {0.0f, 0.0f};
Vec camVel = {0.0f, 0.0f};

world World;
uint8_t worldIndex = 1;

bool quit = false;

bool loadSpritesheets() {
	SDL_Surface* temp = SDL_LoadBMP("../assets/backgrounds/background.bmp");
	if (!temp) {
		printf("error loading background: %s\n", SDL_GetError());
		return false;
	}
	background = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_DestroySurface(temp);

	return true;
}

bool gameInit() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("error initializing sdl: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Renegade", 1600, 900, 0);
	if (!window) {
		printf("error creating window: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		printf("error creating renderer: %s\n", SDL_GetError());
		return false;
	}

	for (uint8_t i = 0; i < 64; i++) {
		World.entities[i] = (entity) {.transform = NULL, .animations = NULL, .hp = NULL, .power = NULL, .defense = NULL, .mass = NULL, .speed = NULL, .cooldown = NULL, .wallCling = NULL, .animationPlaying = NULL, .flags = NULL, .anchored = NULL, .grounded = NULL, .canCollide = NULL, .active = false};
	}

	SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_LINEAR);

	loadSpritesheets();

	return true;
}

void killWindow() {
	cleanData();
	SDL_DestroyTexture(spritesheet);
	spritesheet = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	SDL_Quit();
}

int main() {
	if (!gameInit()) {
		fprintf(stderr, "Game couldn't initalize\n");
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Event eventHandler;

	initPlayer(Idle, "../assets/spritesheets/default_player.png", "../data/animations/player.json", 752.0f, 338.0f, 96.0f, 224.0f, 0, 0, 10, 7, 10);
	newElement(Idle, "../assets/spritesheets/platform.png", "../data/animations/platform.json", 0.0f, 100.0f, 1600.0f, 32.0f, 0, true, true);

	while (!quit) {
		processInput(&eventHandler, &quit);

		printf("x: %f\n", World.entities[0].transform->position.x);
		printf("y: %f\n", World.entities[0].transform->position.y);
		printf("vx: %f\n", World.entities[0].transform->velocity.x);
		printf("vy: %f\n\n", World.entities[0].transform->velocity.y);

		SDL_RenderClear(renderer);

		physicsUpdate();

		SDL_RenderTexture(renderer, background, NULL, NULL);
		printf("began animations\n");
		playAnimations(renderer);
		printf("finished animations\n");
		SDL_RenderPresent(renderer);
	}
	killWindow();

	return 0;
}
