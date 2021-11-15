#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

#define OBSTACLE_SPEED_MIN	

#define OBSTACLE_HEALTH		1
#define OBSTACLE_DAMAGES	1

typedef struct Obstacle 
{
	Entity mEntity;
} Obstacle;

/// <summary>
/// Initialize an obstacle
/// </summary>
/// <param name="obstacle">Obstacle to initialize</param>
void Obstacle_Initialize(Obstacle** obstacle, GameScreenData* gameScreen);

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
void Obstacle_UpdateMovement(Obstacle* obstacle, Game* game);

/// <summary>
/// Check collision of an obstacle
/// </summary>
/// <param name="current">Current obstacle</param>
/// <param name="entity">Other entity which collide with the current </param>
/// <param name="game">Current game</param>
void Obstacle_OnCollide(Obstacle* _current, Entity* entity, Game* game);

/// <summary>
/// Free components of an obstacle and destroy him
/// </summary>
/// <param name="entity">Entity casted to an obstacle to destroy</param>
void Obstacle_Destroy(Entity* entity);
