#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

#define ENEMY_HEALTH_MIN	1
#define ENEMY_HEALTH_MAX	2

#define ENEMY_SPEED_MIN		40
#define ENEMY_SPEED_MAX		50

#define ENEMY_DAMAGES_MIN	1
#define ENEMY_DAMAGES_MAX	1

typedef struct Enemy
{
	Entity mEntity;
}Enemy;

/// <summary>
/// Initializes an enemy
/// </summary>
/// <param name="enemy">Enemy to initialize</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Enemy_Initialize(Enemy** enemy, GameScreenData* gameScreen);

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
void Enemy_UpdateMovement(Enemy* enemy, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Give a random Int between a min and max given in parameters
/// </summary>
int RandomInt(int min, int max);
