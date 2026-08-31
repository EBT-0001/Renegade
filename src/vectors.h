/*
	Renegade  Copyright (C) 2026  Temperlius
    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.
*/

#pragma once

typedef struct {
	float x;
	float y;
} Vec;

typedef struct {
	Vec position;
	Vec scale;
} Box;

typedef struct {
	Vec position;
	Vec velocity;
	Vec scale;
} Transform;
