#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Obstacle 
{
	Entity mEntity;
} Obstacle;

void Obstacle_Initialize(Obstacle** obstacle, GameScreenData* gameScreen);

void Obstacle_Update(Obstacle* obstacle, Game* game, GameScreenData* gameScreen);

void Obstacle_UpdateMovement(Obstacle* obstacle, Game* game, GameScreenData* _gameScreen);
