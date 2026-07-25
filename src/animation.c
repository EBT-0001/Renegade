#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>

#include "vectors.h"
#include "world.h"
#include "animation.h"

void animationConfig(sprite sprite, uint8_t frameCount, animation* animation) {
	FILE* animations;
	animations = fopen("data/animations.bin", "rb");

	animation->frames = malloc(frameCount * sizeof(uint8_t));
	for (uint8_t i = 0; i < frameCount; i++) {
		fseek(animations, sizeof(uint8_t) * sprite, SEEK_SET);

		fread(&animation->frames[i].texCoords.x, sizeof(uint8_t), 1, animations);
		fread(&animation->frames[i].texCoords.y, sizeof(uint8_t), 1, animations);
		fread(&animation->frames[i].texDimensions.x, sizeof(uint8_t), 1, animations);
		fread(&animation->frames[i].texDimensions.y, sizeof(uint8_t), 1, animations);
	}
	fclose(animations);
}
void animationRemove(animation* animation) {
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
		SDL_FRect clip = {
			World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texCoords.x,
			World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texCoords.y,
			World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texDimensions.x,
			World.Player.animations[World.Player.animationPlaying].frames[World.Player.animations[World.Player.animationPlaying].frameClock].texDimensions.y
		};

		SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
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
		if (World.interactables[i].active) {
			SDL_FRect renderQuad = {
				World.interactables[i].transform.position.x,
				World.interactables[i].transform.position.y,
				World.interactables[i].transform.scale.x,
				World.interactables[i].transform.scale.y
			};
			SDL_FRect clip = {
				World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texCoords.x,
				World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texCoords.y,
				World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texDimensions.x,
				World.interactables[i].animations[World.interactables[i].animationPlaying].frames[World.interactables[i].animations[World.interactables[i].animationPlaying].frameClock].texDimensions.y
			};

			SDL_RenderTexture(renderer, spritesheet, &clip, &renderQuad);
		}
	}
}
