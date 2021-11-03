#pragma once

#include "DisplayZoneDrawing.h"
#include "Game.h"

typedef struct Obstacle 
{
	Entity entity;
} Obstacle;

void InitObstacle(Obstacle** obstacle);

void Obstacle_Update(Obstacle* obstacle, struct Game* game);

void Obstacle_UpdateMovement(Obstacle* obstacle, struct Game* game);
