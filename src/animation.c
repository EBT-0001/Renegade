#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cjson/cJSON.h>
#include <SDL3/SDL.h>

#include "vectors.h"
#include "world.h"
#include "animation.h"

void loadAnimation(sprite sprite, animation* animation, const char* path) {
	FILE* animations = fopen(path, "rb");
	if (animations == NULL) {
		printf("Failed to load animation data");
		return;
	}

	char buffer[1024];
	size_t len = fread(buffer, 1, sizeof(buffer) - 1, animations);
	buffer[len] = '\0';
	fclose(animations);

	cJSON* json = cJSON_Parse(buffer);
	if (json == NULL) {
		const char* error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			printf("Error: %s\n", error_ptr);
		}
		cJSON_Delete(json);
		return;
	}
	
	cJSON* animationData;
	switch (sprite) {
		case Idle:
			animationData = cJSON_GetObjectItemCaseSensitive(json, "Idle");
			break;
		default:
			cJSON_Delete(json);
			return;
	}
	if (animationData == NULL) {
		cJSON_Delete(json);
		return;
	}

	animation->frames = malloc(animation->frameCount * sizeof(texture));
	if (animation->frames == NULL) {
		printf("Failed to allocate frames");
		return;
	}
	for (uint8_t i = 0; i < animation->frameCount; i++) {
		animation->frames[i].texCoords.x = cJSON_GetArrayItem(animationData, 4 * i)->valueint;
		animation->frames[i].texCoords.y = cJSON_GetArrayItem(animationData, 4 * i + 1)->valueint;
		animation->frames[i].texDimensions.x = cJSON_GetArrayItem(animationData, 4 * i + 2)->valueint;
		animation->frames[i].texDimensions.y = cJSON_GetArrayItem(animationData, 4 * i + 3)->valueint;
	}
	cJSON_Delete(json);
}
void freeAnimation(animation* animation) {
	free(&(animation->frames));
}
void playAnimations(SDL_Renderer* renderer, SDL_Texture* spritesheet) {
	for (uint8_t i = 0; i < worldIndex; i++) {
		if (World.entities[i].active) {
			SDL_FRect renderQuad = {
				World.entities[i].transform->position.x,
				World.entities[i].transform->position.y,
				World.entities[i].transform->scale.x,
				World.entities[i].transform->scale.y
			};
			SDL_FRect clip = {
				World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texCoords.x,
				World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texCoords.y,
				World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texDimensions.x,
				World.entities[i].animations[*World.entities[i].animationPlaying].frames[World.entities[i].animations[*World.entities[i].animationPlaying].frameClock].texDimensions.y
			};

			SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
			if (World.entities[i].animations[*World.entities[i].animationPlaying].frameClock < World.entities[i].animations[*World.entities[i].animationPlaying].frameCount - 1) {
				World.entities[i].animations[*World.entities[i].animationPlaying].frameClock++;
			} else {
				World.entities[i].animations[*World.entities[i].animationPlaying].frameClock = 0;
			}
		}
	}
}
