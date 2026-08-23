#include <stdio.h>
#include <cjson/cJSON.h>
#include "world.h"

void loadMap(const char* dataPath) {
	FILE* data = fopen(dataPath, "rb");
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
	cJSON* enemyData = cJSON_GetObjectItemCaseSensitive(json, "Enemies");
	cJSON* elementData = cJSON_GetObjectItemCaseSensitive(json, "Elements");

	if (playerData == NULL) {
		cJSON_Delete(json);
		return;
	}
	if (enemyData == NULL) {
		cJSON_Delete(json);
		return;
	}
	if (elementData == NULL) {
		cJSON_Delete(json);
		return;
	}


//	for (uint8_t i = 0; i < playerData[1]; i++) {
//		
//	}
	cJSON_Delete(json);
}
