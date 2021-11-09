#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Obstacle 
{
	Entity mEntity;
} Obstacle;

/// <summary>
/// Initialize an obstacle
/// </summary>
/// <param name="obstacle">Obstacle to initialize</param>
void Obstacle_Initialize(Obstacle** obstacle);

/// <summary>
/// Update an obstacle's movement and appearance
/// </summary>
/// <param name="obstacle">Obstacle to update</param>
/// <param name="game">Game of obstacle</param>
/// <param name="gameScreen">Screen to update</param>
void Obstacle_Update(Obstacle* obstacle, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Update an obstacle's movement
/// </summary>
/// <param name="obstacle">Obstacle to update</param>
/// <param name="game">Game of obstacle</param>
/// <param name="gameScreen">Screen to update</param>
void Obstacle_UpdateMovement(Obstacle* obstacle, Game* game);
