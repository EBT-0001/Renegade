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

	World.Player.transform.position = (Vec) {.x = 0.0f, .y = 0.0f};
	World.Player.transform.velocity = (Vec) {.x = 0.0f, .y = 0.0f};
	World.Player.transform.scale = (Vec) {.x = 12.0f, .y = 28.0f};

	World.Player.animations[0].frameCount = 2;
	loadAnimation(Idle, &World.Player.animations[0], "data/animations/Player.json");

	World.Player.animationPlaying = 0;
	World.Player.animations[0].frameClock = 0;
	World.Player.active = true;
	World.Player.speed = 5;
	World.Player.grounded = false;

	World.elements[0].transform.position = (Vec) {.x = 0.0f, .y = 100.0f};
	World.elements[0].transform.velocity = (Vec) {.x = 0.0f, .y = 0.0f};
	World.elements[0].transform.scale = (Vec) {.x = 32.0f, .y = 16.0f};

	World.elements[0].canCollide = true;
	World.elements[0].active = true;

	loadSpritesheets();

	return true;
}

bool loadSpritesheets() {
	SDL_Surface* temp = SDL_LoadBMP("assets/spritesheet.bmp");
	if (!temp) {
		printf("error loading spritesheet: %s\n", SDL_GetError());
		return false;
	}
	spritesheet = SDL_CreateTextureFromSurface(renderer, temp);

	temp = SDL_LoadBMP("assets/background.bmp");
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
	while (!quit) {
		World.Player.transform.velocity.x = 0.0f;
		processInput(&eventHandler, &quit);

		SDL_RenderClear(renderer);

		physicsUpdatePlayer();

		SDL_RenderTexture(renderer, background, NULL, NULL);
		playAnimations(renderer, spritesheet);
		SDL_RenderPresent(renderer);
	}
	killWindow();

	return 0;
}
