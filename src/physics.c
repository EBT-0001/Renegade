#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "world.h"
#include "animation.h"

float dt = 1.0f/60.0f;
float gravity = 5.5f;

void physicsUpdatePlayer() {
	World.Player.transform.position.x += World.Player.transform.velocity.x * dt;
	World.Player.transform.velocity.y += gravity;
	if (World.Player.transform.velocity.y > gravity) {
		World.Player.transform.velocity.y = gravity;
	}

	World.Player.transform.position.y += World.Player.transform.velocity.y * dt;
	for (uint8_t i = 0; i < 32; i++) {
		if (World.elements[i].active) {
			float playerHalfWidth = World.Player.transform.scale.x/2.0f;
			float playerHalfHeight = World.Player.transform.scale.y/2.0f;
			float objHalfWidth = World.elements[i].transform.scale.x/2.0f;
			float objHalfHeight = World.elements[i].transform.scale.y/2.0f;
			float playerCenterX = World.Player.transform.position.x + playerHalfWidth;
			float playerCenterY = World.Player.transform.position.y + playerHalfHeight;
			float objCenterX = World.elements[i].transform.position.x + objHalfWidth;
			float objCenterY = World.elements[i].transform.position.y + objHalfHeight;
			float diffx = playerCenterX - objCenterX;
			float diffy = playerCenterY - objCenterY;
			float overlapX = (playerHalfWidth + objHalfWidth) - fabs(diffx);
			float overlapY = (playerHalfHeight + objHalfHeight) - fabs(diffy);
			if (overlapX > 0.0f && overlapY > 0.0f && World.elements[i].canCollide && (World.elements[i].anchored || World.elements[i].mass <= World.Player.mass)) {
				if (overlapX < overlapY) {
					World.Player.transform.velocity.x = 0.0f;
					World.Player.transform.velocity.y += 200.0f * dt;
					if (diffx > 0.0f) {
						World.Player.transform.position.x += overlapX;
					} else {
						World.Player.transform.position.x -= overlapX;
					}
				} else {
					World.Player.transform.velocity.y = 0.0f;
					World.Player.transform.velocity.x -= dt;
					if (diffy < 0.0f) {
						World.Player.transform.position.y -= overlapY;
						World.Player.grounded = true;
					} else {
						World.Player.transform.position.y += overlapY;
					}
				}
			}
		}
	}
}

void physicsUpdate() {
	for (uint8_t i = 0; i < worldIndex; i++) {
		if (World.entities[i].active && !World.entities[i].anchored) {
			float obj1x = World.entities[i].transform->position.x;
			float obj1y = World.entities[i].transform->position.x;
			float obj1w = World.entities[i].transform->scale.x;
			float obj1h = World.entities[i].transform->scale.y;
			float obj1vx = World.entities[i].transform->velocity.x;
			float obj1vy = World.entities[i].transform->velocity.y;
			float obj1halfw = obj1w/2.0f;
			float obj1halfh = obj1h/2.0f;
			float obj1centerx = obj1x + obj1halfw;
			float obj1centery = obj1y + obj1halfh;
			for (uint8_t j = 0; j < worldIndex; j++) {
				if (World.entities[j].active && World.entities[j].canCollide) {
					float obj2x = World.entities[j].transform->position.x;
					float obj2y = World.entities[j].transform->position.x;
					float obj2w = World.entities[i].transform->scale.x;
					float obj2h = World.entities[i].transform->scale.y;
					float obj2vx = World.entities[i].transform->velocity.x;
					float obj2vy = World.entities[i].transform->velocity.y;
					float obj2halfw = obj1w/2.0f;
					float obj2halfh = obj1h/2.0f;
					float obj2centerx = obj1x + obj1halfw;
					float obj2centery = obj1y + obj1halfh;
				}
			}
		}
	}
}
