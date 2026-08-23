#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "world.h"
#include "animation.h"

float dt = 1.0f/60.0f;
//float gravity = 30.0f;
float gravity = 0.0f;
float friction = 0.95f;

bool scrollCamera;

void physicsUpdate() {
	for (uint8_t i = 0; i < worldIndex; i++) {
		if (World.entities[i].active && !*World.entities[i].anchored) {
			World.entities[i].transform->position.x += World.entities[i].transform->velocity.x * dt;
			World.entities[i].transform->position.y += World.entities[i].transform->velocity.y * dt;
			World.entities[i].transform->velocity.x *= friction;
			World.entities[i].transform->velocity.y += gravity;
			float obj1x = World.entities[i].transform->position.x;
			float obj1y = World.entities[i].transform->position.x;
			float obj1w = World.entities[i].transform->scale.x;
			float obj1h = World.entities[i].transform->scale.y;
			float obj1halfw = obj1w/2.0f;
			float obj1halfh = obj1h/2.0f;
			float obj1centerx = obj1x + obj1halfw;
			float obj1centery = obj1y + obj1halfh;
			for (uint8_t j = 0; j < worldIndex; j++) {
				if (World.entities[j].active && *World.entities[j].canCollide && (*World.entities[i].mass <= *World.entities[j].mass || *World.entities[j].anchored)) {
					float obj2x = World.entities[j].transform->position.x;
					float obj2y = World.entities[j].transform->position.x;
					float obj2w = World.entities[i].transform->scale.x;
					float obj2h = World.entities[i].transform->scale.y;
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
							if (World.entities[i].wallCling != NULL) World.entities[i].transform->velocity.y -= *World.entities[i].wallCling;
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
	if ((fabs(World.entities[0].transform->velocity.x) > 0.05f || fabs(World.entities[0].transform->velocity.y) > 0.05f) && scrollCamera) {
		deadZone.scale.x = 0.0f;
		deadZone.scale.y = 0.0f;
	} else {
		deadZone.scale.x = 192.0f;
		deadZone.scale.x = 336.0f;
		deadZone.position.x = camera.position.x - 96.0f;
		deadZone.position.y = camera.position.y - 168.0f;
	}
	if (
		World.entities[0].transform->position.x + World.entities[0].transform->scale.x > deadZone.position.x &&
		World.entities[0].transform->position.x < deadZone.position.x  + deadZone.scale.x &&
		World.entities[0].transform->position.y + World.entities[0].transform->scale.y > deadZone.position.y &&
		World.entities[0].transform->position.y < deadZone.position.y + deadZone.scale.y
	) {
		scrollCamera = false;
	} else {
		scrollCamera = true;
	}
	if (scrollCamera) {
		if (World.entities[0].transform->position.x < camera.scale.x/2 + camera.position.x) {
			camera.position.x = (camera.scale.x/2 - 96.0f) + (World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2);
		} else {
			camera.position.x = (camera.scale.x/2 + 96.0f) + (World.entities[0].transform->position.x + World.entities[0].transform->scale.x/2);
		}
		camera.position.y = World.entities[0].transform->position.y + World.entities[0].transform->scale.y/2;
	}
}
