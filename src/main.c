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
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "world.h"
#include "physics.h"
#include "animation.h"
#include "input.h"
#include "map.h"
#include "save.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* background1 = NULL;
SDL_Texture* background2 = NULL;
SDL_Texture* background3 = NULL;

pthread_t input;
pthread_t physics;

Box camera;
Box deadZone;
Box scrollStop;

world World;
uint8_t worldIndex = 1;
int camW, camH;

bool start = false;
bool quit = false;

lua_State* L;

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

	if (!SDL_SetRenderVSync(renderer, 1)) {
		printf("error enabling VSync: %s\n", SDL_GetError());
	}

	for (uint8_t i = 0; i < 64; i++) {
		World.entities[i] = (entity) {.transform = NULL, .animations = NULL, .hp = NULL, .power = NULL, .defense = NULL, .mass = NULL, .speed = NULL, .cooldown = NULL, .wallCling = NULL, .animationPlaying = NULL, .flags = NULL, .anchored = NULL, .grounded = NULL, .canCollide = NULL, .active = false};
	}

	SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_LINEAR);
	SDL_SetWindowFullscreen(window, true);

	SDL_GetWindowSize(window, &camW, &camH);
	camera.scale = (Vec) {camW, camH};
	camera.position = (Vec) {camera.scale.x/2.0f, camera.scale.y/2.0f};

	deadZone.scale = (Vec) {116.0f, 224.0f};
	deadZone.position = (Vec) {camera.position.x - (deadZone.scale.x/2.0f), camera.position.y - (deadZone.scale.y/2.0f)};

	scrollStop.position = (Vec) {0.0f, 0.0f};
	scrollStop.scale = (Vec) {1024.0f, 768.0f};

	L = luaL_newstate();
	luaL_openlibs(L);

	return true;
}

void killWindow() {
	lua_close(L);
	cleanData();
	SDL_DestroyTexture(background1);
	SDL_DestroyTexture(background2);
	SDL_DestroyTexture(background3)
;
	background1 = NULL;
	background2 = NULL;
	background3 = NULL;

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

	loadMap("../data/maps/titlescreen.json");

	pthread_create(&input, NULL, processInput, &eventHandler);
	pthread_create(&physics, NULL, physicsUpdate, NULL);

	int now = 0;
	int last = 0;

	float dt = 0.0f;

	loadSave("../data/saves/save1.json");
	while (!quit) {
		now = SDL_GetPerformanceCounter();

		dt = (float)(now - last)/(float)SDL_GetPerformanceFrequency();
		last = now;

		if (dt * 1000.0f < 1000.0f/60.0f) {
			SDL_Delay((1000.0f/60.0f) - (1000.0f * dt));
		}

		SDL_RenderClear(renderer);

		SDL_GetWindowSize(window, &camW, &camH);
		camera.scale = (Vec) {camW, camH};

		SDL_RenderTexture(renderer, background1, NULL, NULL);
		SDL_RenderTexture(renderer, background2, NULL, NULL);
		SDL_RenderTexture(renderer, background3, NULL, NULL);

		playAnimations(renderer);

		SDL_RenderPresent(renderer);

		cleanData();
	}
	killWindow();

	return 0;
}
