/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "world.h"
#include "map.h"
#include "save.h"
#include "animation.h"

uint8_t parent;

static int getValue(lua_State* L) {
	uint8_t entity = luaL_checkinteger(L, 1);

	const char* value = luaL_checkstring(L, 2);

	if (!strcmp(value, "x")) {
		lua_pushnumber(L, World.entities[entity].transform->position.x);
	} else if (!strcmp(value, "y")) {
		lua_pushnumber(L, World.entities[entity].transform->position.y);
	} else if (!strcmp(value, "width")) {
		lua_pushnumber(L, World.entities[entity].transform->scale.x);
	} else if (!strcmp(value, "height")) {
		lua_pushnumber(L, World.entities[entity].transform->scale.y);
	} else if (!strcmp(value, "hp")) {
		lua_pushinteger(L, *World.entities[entity].hp);
	} else if (!strcmp(value, "power")) {
		lua_pushinteger(L, *World.entities[entity].power);
	} else if (!strcmp(value, "defense")) {
		lua_pushinteger(L, *World.entities[entity].defense);
	} else if (!strcmp(value, "mass")) {
		lua_pushinteger(L, *World.entities[entity].mass);
	} else if (!strcmp(value, "speed")) {
		lua_pushinteger(L, *World.entities[entity].speed);
	} else if (!strcmp(value, "animationPlaying")) {
		lua_pushinteger(L, *World.entities[entity].animationPlaying);
	} else if (!strcmp(value, "active")) {
		lua_pushboolean(L, World.entities[entity].active);
	} else if (!strcmp(value, "mouseX")) {
		lua_pushnumber(L, mouseX);
	} else if (!strcmp(value, "mouseY")) {
		lua_pushnumber(L, mouseY);
	} else if (!strcmp(value, "rightClick")) {
		lua_pushboolean(L, rightClick);
	} else if (!strcmp(value, "leftClick")) {
		lua_pushboolean(L, leftClick);
	}

	return 1;
}

static int getFlag(lua_State* L) {
	uint8_t entity = luaL_checkinteger(L, 1);
	uint8_t index = luaL_checkinteger(L, 2);

	if (World.entities[entity].flags == NULL) {
		World.entities[entity].flags = (bool*) calloc(8, sizeof(bool));
	}

	bool flag = World.entities[entity].flags[index];

	lua_pushboolean(L, flag);

	return 1;
}

static int setValue(lua_State* L) {
	uint8_t entity = luaL_checkinteger(L, 1);

	const char* value = luaL_checkstring(L, 2);

	if (!strcmp(value, "x")) {
		World.entities[entity].transform->position.x = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "y")) {
		World.entities[entity].transform->position.y = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "width")) {
		World.entities[entity].transform->scale.x = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "height")) {
		World.entities[entity].transform->scale.y = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "hp")) {
		*World.entities[entity].hp = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "power")) {
		*World.entities[entity].power = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "defense")) {
		*World.entities[entity].defense = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "mass")) {
		*World.entities[entity].mass = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "speed")) {
		*World.entities[entity].speed = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "animationPlaying")) {
		*World.entities[entity].animationPlaying = luaL_checkinteger(L, 3);
	} else if (!strcmp(value, "active")) {
		World.entities[entity].active = lua_toboolean(L, 3);
	}

	return 0;
}

static int setFlag(lua_State* L) {
	uint8_t entity = luaL_checkinteger(L, 1);
	uint8_t index = luaL_checkinteger(L, 2);

	if (World.entities[entity].flags == NULL) {
		World.entities[entity].flags = (bool*) calloc(8, sizeof(bool));
	}

	World.entities[entity].flags[index] = lua_toboolean(L, 3);

	return 0;
}

static int loadAnimationLua(lua_State* L) {
	uint8_t sprite = luaL_checkinteger(L, 1);
	uint8_t i = luaL_checkinteger(L, 2);

	const char* spritePath = luaL_checkstring(L, 3);
	const char* dataPath = luaL_checkstring(L, 4);

	loadAnimation(renderer, sprite, &World.entities[parent].animations[i], spritePath, dataPath);

	return 0;
}

static int newElementLua(lua_State* L) {
	const char* spritePath = luaL_checkstring(L, 1);
	const char* animationPath = luaL_checkstring(L, 2);

	float x = luaL_checknumber(L, 3);
	float y = luaL_checknumber(L, 4);
	float width = luaL_checknumber(L, 5);
	float height = luaL_checknumber(L, 6);
	uint8_t mass = luaL_checkinteger(L, 7);
	bool canCollide = lua_toboolean(L, 8);
	bool anchored = lua_toboolean(L, 9);

	const char* script = luaL_checkstring(L, 10);

	newElement(spritePath, animationPath, x, y, width, height, mass, canCollide, anchored, script);

	return 0;
}

static int newEnemyLua(lua_State* L) {
	const char* spritePath = luaL_checkstring(L, 1);
	const char* animationPath = luaL_checkstring(L, 2);

	float x = luaL_checknumber(L, 3);
	float y = luaL_checknumber(L, 4);
	float width = luaL_checknumber(L, 5);
	float height = luaL_checknumber(L, 6);
	uint8_t power = luaL_checkinteger(L, 7);
	uint8_t defense = luaL_checkinteger(L, 8);
	uint8_t mass = luaL_checkinteger(L, 9);
	uint8_t speed = luaL_checkinteger(L, 10);

	const char* script = luaL_checkstring(L, 11);

	newEnemy(spritePath, animationPath, x, y, width, height, power, defense, mass, speed, script);

	return 0;
}

static int loadMapLua(lua_State* L) {
	const char* map = luaL_checkstring(L, 1);

	loadMap(map);

	return 0;
}

static int loadSaveLua(lua_State* L) {
	const char* save = luaL_checkstring(L, 1);

	loadSave(save);

	return 0;
}

void registerFunctions() {
	lua_pushcfunction(L, getValue);
	lua_setglobal(L, "getValue");

	lua_pushcfunction(L, getFlag);
	lua_setglobal(L, "getFlag");

	lua_pushcfunction(L, setValue);
	lua_setglobal(L, "setValue");

	lua_pushcfunction(L, setFlag);
	lua_setglobal(L, "setFlag");

	lua_pushcfunction(L, loadAnimationLua);
	lua_setglobal(L, "loadAnimation");

	lua_pushcfunction(L, newElementLua);
	lua_setglobal(L, "newElement");

	lua_pushcfunction(L, newEnemyLua);
	lua_setglobal(L, "newEnemy");

	lua_pushcfunction(L, loadMapLua);
	lua_setglobal(L, "loadMap");

	lua_pushcfunction(L, loadSaveLua);
	lua_setglobal(L, "loadSave");
}

void* runFunctions(void* arg) {
	(void)arg;

	lua_pushnumber(L, 0);
	lua_setglobal(L, "Idle");

	lua_pushnumber(L, 1);
	lua_setglobal(L, "Idle2");

	int now = 0;
	int last = 0;

	float dt = 0;
	while (!quit) {
		now = SDL_GetPerformanceCounter();

		dt = (float)(now - last)/(float)SDL_GetPerformanceFrequency();
		last = now;

		if (1000.0f * dt < 1000.0f/60.0f) {
			SDL_Delay((1000.0f/60.0f) - (1000.0f * dt));
		}
		for (parent = 0; parent < 64; parent++) {
			if (World.entities[parent].active) {
				if (strcmp(World.entities[parent].script, "none")) {
					lua_pushinteger(L, parent);
					lua_setglobal(L, "parent");

//					int result = luaL_dofile(L, World.entities[parent].script);
					int result = luaL_dofile(L, "../scripts/load.lua");

					if (result != LUA_OK) {
						fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
						lua_pop(L, 1);
					}
				}
			}
		}
	}
	return NULL;
}
