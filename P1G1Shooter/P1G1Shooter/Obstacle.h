#pragma once

#include "DisplayZoneDrawing.h"

#define DISPLAY_OBSTACLE 'X'

enum Direction {
	TOP,
	RIGHT,
	BOTTOM,
	LEFT,
};

typedef Obstacle(*Init)(double speed, int damage);
typedef void (*Move)(Obstacle obstacle, Direction direction); // Direction potentiellement à retirer au 
															  // profit d'un pattern précis
															  // pour les projectiles
typedef struct Entity {
	DisplayZone mSprite;
};

typedef struct Obstacle {

	Entity mEntity;

	double mSpeed;
	int mDamage;

	Move move;
} Obstacle;

