#pragma once

#include "Game.h"

typedef struct Obstacle 
{
	Entity entity;
} Obstacle;

void InitObstacle(Obstacle** obstacle);

void Obstacle_Update(Obstacle* obstacle, Game* game);

void Obstacle_UpdateMovement(Obstacle* obstacle, Game* game);
