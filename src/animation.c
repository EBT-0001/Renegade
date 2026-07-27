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
	if (World.Player.active) {
		SDL_FRect renderQuad = {
			World.Player.transform.position.x,
			World.Player.transform.position.y,
			World.Player.transform.scale.x,
			World.Player.transform.scale.y
		};
//		SDL_FRect clip = {
			printf("%d\n",World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texCoords.x);
			printf("%d\n",World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texCoords.y);
			printf("%d\n",World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texDimensions.x);
			printf("%d\n",World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texDimensions.y);
//		};

//		SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
		if (World.Player.animations[World.Player.animationPlaying].frameClock < World.Player.animations[World.Player.animationPlaying].frameCount - 1) {
			World.Player.animations[World.Player.animationPlaying].frameClock++;
		} else {
			World.Player.animations[World.Player.animationPlaying].frameClock = 0;
		}
	}
	for (uint8_t i = 0; i < 16; i++) {
		if (World.enemies[i].active) {
			SDL_FRect renderQuad = {
				World.enemies[i].transform.position.x,
				World.enemies[i].transform.position.y,
				World.enemies[i].transform.scale.x,
				World.enemies[i].transform.scale.y
			};
			SDL_FRect clip = {
				World.enemies[i].animations[World.enemies[i].animationPlaying].frames[World.enemies[i].animations[World.enemies[i].animationPlaying].frameClock].texCoords.x,
				World.enemies[i].animations[World.enemies[i].animationPlaying].frames[World.enemies[i].animations[World.enemies[i].animationPlaying].frameClock].texCoords.y,
				World.enemies[i].animations[World.enemies[i].animationPlaying].frames[World.enemies[i].animations[World.enemies[i].animationPlaying].frameClock].texDimensions.x,
				World.enemies[i].animations[World.enemies[i].animationPlaying].frames[World.enemies[i].animations[World.enemies[i].animationPlaying].frameClock].texDimensions.y
			};

			SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
		}
	}
	for (uint8_t i = 0; i < 32; i++) {
		if (World.elements[i].active) {
			SDL_FRect renderQuad = {
				World.elements[i].transform.position.x,
				World.elements[i].transform.position.y,
				World.elements[i].transform.scale.x,
				World.elements[i].transform.scale.y
			};
			SDL_FRect clip = {
				World.elements[i].animation.frames[World.elements[i].animation.frameClock].texCoords.x,
				World.elements[i].animation.frames[World.elements[i].animation.frameClock].texCoords.y,
				World.elements[i].animation.frames[World.elements[i].animation.frameClock].texDimensions.x,
				World.elements[i].animation.frames[World.elements[i].animation.frameClock].texDimensions.y
			};

			SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
		}
	}
	for (uint8_t i =0; i < 8; i++) {
		//if (World.interactables[i].active) {
		//	SDL_FRect renderQuad = {
		//		World.interactables[i].transform.position.x,
		//		World.interactables[i].transform.position.y,
		//		World.interactables[i].transform.scale.x,
		//		World.interactables[i].transform.scale.y
		//	};
		//	SDL_FRect clip = {
		//		World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texCoords.x,
		//		World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texCoords.y,
		//		World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texDimensions.x,
		//		World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texDimensions.y
		//	};

		//	SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
		//}
	}
}
