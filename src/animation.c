#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "world.h"
#include "animation.h"

SDL_Texture* loadAnimation(SDL_Renderer* renderer, sprite sprite, animation* animation, const char* spritePath, const char* dataPath) {
	FILE* data = fopen(dataPath, "rb");
	if (data == NULL) {
		printf("Failed to load animation data");
		return NULL;
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
		return NULL;
	}

	cJSON* animationData;
	switch (sprite) {
		case Idle:
			animationData = cJSON_GetObjectItemCaseSensitive(json, "Idle");
			break;
		default:
			cJSON_Delete(json);
			return NULL;
	}
	if (animationData == NULL) {
		cJSON_Delete(json);
		return NULL;
	}

	animation->frames = (texture*) malloc(animation->frameCount * sizeof(texture));
	if (animation->frames == NULL) {
		printf("Failed to allocate frames");
		return NULL;
	}
	for (uint8_t i = 0; i < animation->frameCount; i++) {
		animation->frames[i].texCoords.x = cJSON_GetArrayItem(animationData, 4 * i)->valueint;
		animation->frames[i].texCoords.y = cJSON_GetArrayItem(animationData, 4 * i + 1)->valueint;
		animation->frames[i].texDimensions.x = cJSON_GetArrayItem(animationData, 4 * i + 2)->valueint;
		animation->frames[i].texDimensions.y = cJSON_GetArrayItem(animationData, 4 * i + 3)->valueint;
	}
	cJSON_Delete(json);
	SDL_Surface* temp = IMG_Load(spritePath);
	if (!temp) {
		printf("error loading spritesheet: %s\n", SDL_GetError());
		return NULL;
	}
	return SDL_CreateTextureFromSurface(renderer, temp);
}
void freeAnimation(animation* animation) {
	free(&(animation->frames));
}
void* playAnimations(void* arg) {
	SDL_Renderer* renderer = (SDL_Renderer*) arg;
	while (!quit) {
		for (uint8_t i = 0; i < worldIndex; i++) {
			if (World.entities[i].active) {
				SDL_FRect renderQuad = {
					World.entities[i].transform->position.x - camera.position.x,
					World.entities[i].transform->position.y - camera.position.y,
					World.entities[i].transform->scale.x,
					World.entities[i].transform->scale.y
				};
				SDL_FRect clip = {
					World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texCoords.x,
					World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texCoords.y,
					World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texDimensions.x,
					World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texDimensions.y
				};

				SDL_RenderTexture(renderer, World.entities[i].spritesheet, &clip, &renderQuad);
				if (World.entities[i].animations[*World.entities[i].animationPlaying].frameClock < World.entities[i].animations[*World.entities[i].animationPlaying].frameCount - 1) {
					World.entities[i].animations[*World.entities[i].animationPlaying].frameClock++;
				} else {
					World.entities[i].animations[*World.entities[i].animationPlaying].frameClock = 0;
				}
			}
		}
	}
}
