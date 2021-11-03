#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Obstacle 
{
	Entity entity;
} Obstacle;

void InitObstacle(Obstacle** obstacle);

void Obstacle_Update(Obstacle* obstacle, Game* game, GameScreenData* gameScreen);

void Obstacle_UpdateMovement(Obstacle* obstacle, Game* game, GameScreenData* _gameScreen);
