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
void physicsUpdateEnemy() {
	for (uint8_t i = 0; i < 16; i++) {
		World.enemies[i].transform.position.x += World.enemies[i].transform.velocity.x * dt;
		World.enemies[i].transform.velocity.y -= gravity * dt;
		World.enemies[i].transform.position.y += World.enemies[i].transform.velocity.y * dt;
		float enemyHalfWidth = World.enemies[i].transform.scale.x/2.0f;
		float enemyHalfHeight = World.enemies[i].transform.scale.y/2.0f;
		float enemyCenterX = World.enemies[i].transform.position.x + enemyHalfWidth;
		float enemyCenterY = World.enemies[i].transform.position.y + enemyHalfHeight;
		for (uint8_t j = 0; j < 32; j++) {
			float objHalfWidth = World.elements[j].transform.scale.x/2.0f;
			float objHalfHeight = World.elements[j].transform.scale.y/2.0f;
			float objCenterX = World.elements[j].transform.position.x + objHalfWidth;
			float objCenterY = World.elements[j].transform.position.y + objHalfHeight;
			float diffx = enemyCenterX - objCenterX;
			float diffy = enemyCenterY - objCenterY;
			if ((fabs(diffx) < enemyHalfWidth + objHalfWidth) && (fabs(diffy) < enemyHalfHeight + objHalfHeight) && World.elements[i].canCollide && (World.elements[i].anchored || World.elements[j].mass <= World.enemies[i].mass)) {
				if (fabs(diffx) < fabs(diffy)) {
					World.enemies[i].transform.position.x += diffx;
					World.enemies[i].transform.velocity.x = 0.0f;
					World.enemies[i].transform.velocity.y += 50.0f * dt;
				} else {
					World.enemies[i].transform.position.y += diffy;
					World.enemies[i].transform.velocity.y = 0.0f;
					World.enemies[i].transform.velocity.x -= 50.0f * dt;
				}
			}
		}
		float playerHalfWidth = World.Player.transform.scale.x/2.0f;
		float playerHalfHeight = World.Player.transform.scale.y/2.0f;
		float playerCenterX = World.Player.transform.position.x + playerHalfWidth;
		float playerCenterY = World.Player.transform.position.y + playerHalfHeight;
		float diffx = playerCenterX - enemyCenterX;
		float diffy = playerCenterY - enemyCenterY;
		if ((fabs(diffx) < enemyHalfWidth + playerHalfWidth) && (fabs(diffy) < enemyHalfHeight + playerHalfHeight)) {
			if (World.Player.cooldown <= 0) {
				World.Player.hp -= World.enemies[i].power - World.enemies[i].power * (1/World.Player.defense);
				World.Player.cooldown += 120;
				if (fabs(World.enemies[i].transform.velocity.x) > 0.0f) {
					if (World.enemies[i].transform.velocity.x > 0) {
						World.Player.transform.velocity.x += World.enemies[i].knockback.x;
					} else {
						World.Player.transform.velocity.x -= World.enemies[i].knockback.x;
					}
				} else {
					if (diffx > 0.0f) {
						World.Player.transform.velocity.x += World.enemies[i].knockback.x;
					} else {
						World.Player.transform.velocity.x -= World.enemies[i].knockback.x;
					}
				}
				World.Player.transform.velocity.y += World.enemies[i].knockback.y;
			}
		}
	}
}
