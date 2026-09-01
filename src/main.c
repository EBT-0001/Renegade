/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/
	
#include <pthread.h>
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
SDL_Texture* background1 = NULL;
SDL_Texture* background2 = NULL;
SDL_Texture* background3 = NULL;

pthread_t input;
pthread_t graphics;
pthread_t physics;

Box camera;
Box deadZone;
Box scrollStop;

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

	window = SDL_CreateWindow("Renegade", camera.scale.x, camera.scale.y, 0);
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

	camera.position = (Vec) {512.0f, 384.0f};
	camera.scale = (Vec) {1024.0f, 768.0f};

	deadZone.position = (Vec) {416.0f, 216.0f};
	deadZone.scale = (Vec) {192.0f, 336.0f};

	scrollStop.position = (Vec) {0.0f, 0.0f};
	scrollStop.scale = (Vec) {1024.0f, 768.0f};

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

	pthread_exit(NULL);
}

int main() {
	if (!gameInit()) {
		fprintf(stderr, "Game couldn't initalize\n");
		return 1;
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_Event eventHandler;

	initPlayer(Idle, "../assets/spritesheets/default_player.png", "../data/animations/player.json", camera.scale.x/2, camera.scale.y/2, 96.0f, 224.0f, 0, 0, 10, 85, 10);
	newElement(Idle, "../assets/spritesheets/platform.png", "../data/animations/platform.json", 0.0f, 736.0f, 1024.0f, 64.0f, 0, true, true);
	newElement(Idle, "../assets/spritesheets/platform.png", "../data/animations/platform.json", 1024.0f, 736.0f, 1024.0f, 64.0f, 0, true, true);
	newElement(Idle, "../assets/spritesheets/platform.png", "../data/animations/platform.json", 2048.0f, 736.0f, 1024.0f, 64.0f, 0, true, true);

	pthread_create(&input, NULL, processInput, &eventHandler);
	pthread_create(&graphics, NULL, playAnimations, renderer);
	pthread_create(&physics, NULL, physicsUpdate, NULL);

	while (!quit) {
		SDL_RenderClear(renderer);

		SDL_RenderTexture(renderer, background1, NULL, NULL);

		SDL_RenderPresent(renderer);

		cleanData();
	}
	killWindow();

	return 0;
}
