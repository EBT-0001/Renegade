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
//	cJSON* playerData;
	cJSON* mapData = cJSON_GetObjectItemCaseSensitive(json, "Map");

	loadMap(mapData->valuestring);
}
