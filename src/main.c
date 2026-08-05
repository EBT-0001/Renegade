#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "world.h"
#include "physics.h"
#include "animation.h"
#include "input.h"
#include "game.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* spritesheet = NULL;
SDL_Texture* background = NULL;

world World;
uint8_t worldIndex = 0;

bool quit = false;

bool gameInit() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		printf("error initializing sdl: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Renegade", 1280, 720, 0);
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

	loadSpritesheets();

	return true;
}

bool loadSpritesheets() {
	SDL_Surface* temp = SDL_LoadBMP("../assets/spritesheet.bmp");
	if (!temp) {
		printf("error loading spritesheet: %s\n", SDL_GetError());
		return false;
	}
	spritesheet = SDL_CreateTextureFromSurface(renderer, temp);

	temp = SDL_LoadBMP("../assets/background.bmp");
	if (!temp) {
		printf("error loading background: %s\n", SDL_GetError());
		return false;
	}
	background = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_DestroySurface(temp);
	if (!spritesheet) {
		printf("error converting spritesheet: %s\n", SDL_GetError());
		return false;
	}
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

	initPlayer(Idle, "../data/animations/player.json", 0.0f, 0.0f, 12.0f, 28.0f, 0, 0, 10, 10, 10);

	while (!quit) {
		processInput(&eventHandler, &quit);

		SDL_RenderClear(renderer);

		physicsUpdate();

		SDL_RenderTexture(renderer, background, NULL, NULL);
		playAnimations(renderer, spritesheet);
		SDL_RenderPresent(renderer);
	}
	killWindow();

	return 0;
}
