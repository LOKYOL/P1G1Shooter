#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

#define ENEMYK_HEALTH_MIN	1
#define ENEMYK_HEALTH_MAX	2

#define ENEMYK_SPEED_MIN	40
#define ENEMYK_SPEED_MAX	50

#define ENEMYK_DAMAGES		1

typedef struct EnemyKamikaze
{
	Entity mEntity;
}EnemyKamikaze;

/// <summary>
/// Initializes an enemy
/// </summary>
/// <param name="enemy">Enemy to initialize</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Enemy_Initialize(EnemyKamikaze** enemy, GameScreenData* gameScreen);

/// <summary>
/// Update an enemy
/// </summary>
/// <param name="enemy">Enemy to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Enemy_Update(void* enemy, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of an enemy
/// </summary>
/// <param name="enemy">Enemy to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void Enemy_UpdateMovement(EnemyKamikaze* enemy, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Check collision of an ennemy kamikaze
/// </summary>
/// <param name="current">Current ennemy kamikaze</param>
/// <param name="entity">Other entity which collide with the current </param>
/// <param name="game">Current game</param>
void Enemy_OnCollide(EnemyKamikaze* current, Entity* entity, Game* game);

/// <summary>
/// Free components of an enemy and destroy him
/// </summary>
/// <param name="entity">Entity casted to an enemy to destroy</param>
void Enemy_Destroy(Entity* entity);

/// <summary>
/// Give a random Int between a min and max given in parameters
/// </summary>
int RandomInt(int min, int max);
