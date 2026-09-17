/*
	Renegade  Copyright (C) 2026  Temperlius
	This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
	This is free software, and you are welcome to redistribute it
	under certain conditions; type `show c' for details.
*/

#include <stdio.h>
#include <cjson/cJSON.h>

#include "world.h"
#include "map.h"

void loadSave(const char* save) {
	FILE* data = fopen(save, "rb");
	if (data == NULL) {
		printf("Failed to load animation data");
		return;
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
		return;
	}
	cJSON* playerData = cJSON_GetObjectItemCaseSensitive(json, "Player");
	cJSON* mapData = cJSON_GetObjectItemCaseSensitive(json, "Map");

	loadMap(mapData->valuestring);

	if (playerData == NULL) {
		cJSON_Delete(json);
		return;
	}

	initPlayer(
		cJSON_GetArrayItem(playerData, 0)->valuestring,
		cJSON_GetArrayItem(playerData, 1)->valuestring,
		cJSON_GetArrayItem(playerData, 2)->valuedouble,
		cJSON_GetArrayItem(playerData, 3)->valuedouble,
		cJSON_GetArrayItem(playerData, 4)->valuedouble,
		cJSON_GetArrayItem(playerData, 5)->valuedouble,
		cJSON_GetArrayItem(playerData, 6)->valueint,
		cJSON_GetArrayItem(playerData, 7)->valueint,
		cJSON_GetArrayItem(playerData, 8)->valueint,
		cJSON_GetArrayItem(playerData, 9)->valueint,
		cJSON_GetArrayItem(playerData, 10)->valueint,
		cJSON_GetArrayItem(playerData, 11)->valuestring
	);
	start = true;
}
