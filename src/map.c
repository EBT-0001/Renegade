/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#include <stdio.h>
#include <cjson/cJSON.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include "world.h"

void loadMap(const char* dataPath) {
	FILE* data = fopen(dataPath, "rb");
	if (data == NULL) {
		printf("Failed to load animation data");
		return;
	}

	char buffer[1024];
	size_t len = fread(buffer, 1, sizeof(buffer) - 1, data);
	buffer[len] = '\0';
	fclose(data);

	cJSON* json = cJSON_Parse(buffer);
	if (json == NULL) {
		const char* error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			printf("Error: %s\n", error_ptr);
		}
		cJSON_Delete(json);
		return;
	}

	cJSON* playerData = cJSON_GetObjectItemCaseSensitive(json, "Player");
	cJSON* enemyData = cJSON_GetObjectItemCaseSensitive(json, "Enemies");
	cJSON* elementData = cJSON_GetObjectItemCaseSensitive(json, "Elements");
	cJSON* backgroundData = cJSON_GetObjectItemCaseSensitive(json, "Backgrounds");

	if (playerData == NULL) {
		cJSON_Delete(json);
		return;
	}
	if (enemyData == NULL) {
		cJSON_Delete(json);
		return;
	}
	if (elementData == NULL) {
		cJSON_Delete(json);
		return;
	}
	if (backgroundData == NULL) {
		cJSON_Delete(json);
		return;
	}

	for (uint8_t i = 0; i < cJSON_GetArraySize(elementData); i += 10) {
		newElement(
			cJSON_GetArrayItem(elementData, i)->valueint,
			cJSON_GetArrayItem(elementData, i + 1)->valuestring,
			cJSON_GetArrayItem(elementData, i + 2)->valuestring,
			cJSON_GetArrayItem(elementData, i + 3)->valuedouble,
			cJSON_GetArrayItem(elementData, i + 4)->valuedouble,
			cJSON_GetArrayItem(elementData, i + 5)->valuedouble,
			cJSON_GetArrayItem(elementData, i + 6)->valuedouble,
			cJSON_GetArrayItem(elementData, i + 7)->valueint,
			cJSON_GetArrayItem(elementData, i + 8)->valueint,
			cJSON_GetArrayItem(elementData, i + 9)->valueint
		);
	}
	SDL_Surface* temp = SDL_LoadBMP(cJSON_GetArrayItem(backgroundData, 0)->valuestring);
	if (!temp) {
		printf("error loading background: %s\n", SDL_GetError());
		return;
	}
	background1 = SDL_CreateTextureFromSurface(renderer, temp);
	temp = IMG_Load(cJSON_GetArrayItem(backgroundData, 1)->valuestring);
	if (!temp) {
		printf("error loading background: %s\n", SDL_GetError());
		return;
	}
	background2 = SDL_CreateTextureFromSurface(renderer, temp);
	temp = IMG_Load(cJSON_GetArrayItem(backgroundData, 2)->valuestring);
	if (!temp) {
		printf("error loading background: %s\n", SDL_GetError());
		return;
	}
	background3 = SDL_CreateTextureFromSurface(renderer, temp);

	SDL_DestroySurface(temp);
	cJSON_Delete(json);
}
