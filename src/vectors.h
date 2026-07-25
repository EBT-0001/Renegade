#pragma once

typedef struct {
	float x;
	float y;
} Vec;

typedef struct {
	Vec position;
	Vec velocity;
	Vec scale;
} Transform;
