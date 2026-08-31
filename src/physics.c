#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "world.h"
#include "animation.h"

float dt = 1.0f/60.0f;
float gravity = 30.0f;
float friction = 0.95f;

Vec adjustRate = (Vec) {0.0f, 0.0f};
bool adjustRateCalculated = false;

bool scrollCamera;

void* physicsUpdate(void* arg) {
	while (!quit) {
		for (uint8_t i = 0; i < worldIndex; i++) {
			if (World.entities[i].active && !*World.entities[i].anchored) {
				World.entities[i].transform->position.x += World.entities[i].transform->velocity.x * dt;
				World.entities[i].transform->position.y += World.entities[i].transform->velocity.y * dt;
				World.entities[i].transform->velocity.x *= friction;
				World.entities[i].transform->velocity.y += gravity;
				float obj1x = World.entities[i].transform->position.x;
				float obj1y = World.entities[i].transform->position.y;
				float obj1w = World.entities[i].transform->scale.x;
				float obj1h = World.entities[i].transform->scale.y;
				float obj1halfw = obj1w/2.0f;
				float obj1halfh = obj1h/2.0f;
				float obj1centerx = obj1x + obj1halfw;
				float obj1centery = obj1y + obj1halfh;
				for (uint8_t j = 0; j < worldIndex; j++) {
					if (
						i != j &&
						World.entities[j].active &&
						*World.entities[j].canCollide &&
						(*World.entities[i].mass <= *World.entities[j].mass ||
						*World.entities[j].anchored)
					) {
						float obj2x = World.entities[j].transform->position.x;
						float obj2y = World.entities[j].transform->position.y;
						float obj2w = World.entities[j].transform->scale.x;
						float obj2h = World.entities[j].transform->scale.y;
						float obj2halfw = obj2w/2.0f;
						float obj2halfh = obj2h/2.0f;
						float obj2centerx = obj2x + obj2halfw;
						float obj2centery = obj2y + obj2halfh;
						float diffx = obj1centerx - obj2centerx;
						float diffy = obj1centery - obj2centery;
						float overlapX = (obj1halfw + obj2halfw) - fabs(diffx);
						float overlapY = (obj1halfh + obj2halfh) - fabs(diffy);
						if (overlapX > 0.0f && overlapY > 0.0f) {
							if (overlapX < overlapY) {
								World.entities[i].transform->velocity.x = 0.0f;
								if (World.entities[i].wallCling != NULL) {
									World.entities[i].transform->velocity.y -= *World.entities[i].wallCling;
								}
								if (diffx > 0.0f) {
									World.entities[i].transform->position.x += overlapX;
								} else {
									World.entities[i].transform->position.x -= overlapX;
								}
							} else {
								World.entities[i].transform->velocity.y = 0.0f;
								if (diffy < 0.0f) {
									if (World.entities[i].grounded != NULL) *World.entities[i].grounded = true;
									World.entities[i].transform->position.y -= overlapY;
								} else {
									World.entities[i].transform->position.y += overlapY;
								}
							}
						}
					}
				}
			}
		}

		scrollCamera = true;

		if (
			World.entities[0].transform->position.x < deadZone.position.x + deadZone.scale.x &&
			World.entities[0].transform->position.x + World.entities[0].transform->scale.x > deadZone.position.x &&
			World.entities[0].transform->position.y < deadZone.position.y + deadZone.scale.y &&
			World.entities[0].transform->position.y + World.entities[0].transform->scale.y > deadZone.position.y
		) {
			scrollCamera = false;
		} else if (fabs(World.entities[0].transform->velocity.x) < *World.entities[0].speed && fabs(World.entities[0].transform->velocity.y) < gravity) {
			deadZone.position.x = (World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) - deadZone.scale.x/2.0f;
			deadZone.position.y = (World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f) - deadZone.scale.y/2.0f;
			deadZone.scale.x = 192.0f;
			deadZone.scale.y = 336.0f;

			scrollCamera = false;
		}

		if (scrollCamera) {
			deadZone.scale = (Vec) {0.0f, 0.0f};
			if (
				fabs((World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) - (camera.position.x + camera.scale.x/2.0f)) < 0.5f &&
				fabs((World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f) - (camera.position.y + camera.scale.y/2.0f)) < 0.5f
			) {
				camera.position.x = (World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) - camera.scale.x/2.0f;
				camera.position.y = (World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f) - camera.scale.y/2.0f;

				adjustRateCalculated = false;
			} else {
				if (adjustRateCalculated) {
					camera.position.x += World.entities[0].transform->velocity.x * dt;
					camera.position.y += World.entities[0].transform->velocity.y * dt;

					camera.position.x += adjustRate.x;
					camera.position.y += adjustRate.y;
					if (
						(adjustRate.x < 0 && camera.position.x + camera.scale.x/2.0f < World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) ||
						(adjustRate.x > 0 && camera.position.x + camera.scale.x/2.0f > World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) ||
						(adjustRate.y < 0 && camera.position.y + camera.scale.y/2.0f < World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f) ||
						(adjustRate.y > 0 && camera.position.y + camera.scale.y/2.0f > World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f)
					) {
						adjustRateCalculated = false;
					}
				} else {
					adjustRate.x = ((World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2.0f) - (camera.position.x + camera.scale.x/2.0f)) * dt;
					adjustRate.y = ((World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2.0f) - (camera.position.y + camera.scale.y/2.0f)) * dt;

					adjustRateCalculated = true;
				}
			}
		}
	}
}
