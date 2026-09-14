/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/

#include <string.h>
#include <stdint.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "world.h"

static int getValue(lua_State* L) {
	uint8_t entity = luaL_checknumber(L, 1);
	char* value;

	value = luaL_checkstring(L, 2);

	if (!strcmp(value, "x")) {
		lua_pushnumber(L, World.entities[entity].transform->position.x);
	} else if (!strcmp(value, "y")) {
		lua_pushnumber(L, World.entities[entity].transform->position.y);
	} else if (!strcmp(value, "width")) {
		lua_pushnumber(L, World.entities[entity].transform->scale.x);
	} else if (!strcmp(value, "height")) {
		lua_pushnumber(L, World.entities[entity].transform->scale.y);
	} else if (!strcmp(value, "hp")) {
		lua_pushnumber(L, *World.entities[entity].hp);
	} else if (!strcmp(value, "power")) {
		lua_pushnumber(L, *World.entities[entity].power);
	} else if (!strcmp(value, "defense")) {
		lua_pushnumber(L, *World.entities[entity].defense);
	} else if (!strcmp(value, "mass")) {
		lua_pushnumber(L, *World.entities[entity].mass);
	} else if (!strcmp(value, "speed")) {
		lua_pushnumber(L, *World.entities[entity].speed);
	} else if (!strcmp(value, "animationPlaying")) {
		lua_pushnumber(L, *World.entities[entity].animationPlaying);
	} else if(!strcmp(value, "active")) {
		lua_pushboolean(L, World.entities[entity].active);
	}

	return 1;
}

static int setValue(lua_State* L) {
	uint8_t entity = luaL_checknumber(L, 1);
	char* value;

	value = luaL_checkstring(L, 2);

	if (!strcmp(value, "x")) {
		World.entities[entity].transform->position.x = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "y")) {
		World.entities[entity].transform->position.y = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "width")) {
		World.entities[entity].transform->scale.x = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "height")) {
		World.entities[entity].transform->scale.y = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "hp")) {
		*World.entities[entity].hp = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "power")) {
		*World.entities[entity].power = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "defense")) {
		*World.entities[entity].defense = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "mass")) {
		*World.entities[entity].mass = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "speed")) {
		*World.entities[entity].speed = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "animationPlaying")) {
		*World.entities[entity].animationPlaying = luaL_checknumber(L, 3);
	} else if (!strcmp(value, "active")) {
		World.entities[entity].active = lua_toboolean(L, 3);
	}

	return 0;
}

static int newElementLua(lua_State* L) {
	char* spritePath, animationPath;
	spritePath = luaL_checkstring(L, 1);
	animationPath = luaL_checkstring(L, 2);

	float x = luaL_checknumber(L, 3);
	float y = luaL_checknumber(L, 4);
	float width = luaL_checknumber(L, 5);
	float height = luaL_checknumber(L, 6);
	uint8_t power = luaL_checknumber(L, 7);
	uint8_t defense = luaL_checknumber(L, 8);
	uint8_t mass = luaL_checknumber(L, 9);
	uint8_t speed = luaL_checknumber(L, 10);

	newElement(spritePath, animationPath, x, y, width, height, power, defense, mass, speed);
}

void registerFunctions() {
	lua_pushcfunction(L, getValue);
	lua_setglobal(L, "getValue");

	lua_pushcfunction(L, setValue);
	lua_setglobal(L, "setValue");
}

void runFunctions() {
	for (uint8_t i = 0; i < 64; i++) {
		if (World.entities[i].script != NULL) {
			int result = luaL_dofile(L, World.entities[i].script);

			if (result != LUA_OK) {
				fprintf(stderr, "Lua error: %s\n", lua_tostring(L, -1));
				lua_pop(L, 1);
			}
		}
	}
}
