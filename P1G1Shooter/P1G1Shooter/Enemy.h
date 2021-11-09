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
void Enemy_Initialize(Enemy** _enemy, GameScreenData* gameScreen);

/// <summary>
/// Change the position of an enemy
/// </summary>
/// <param name="enemy">Enemy to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Enemy_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen);

/// <summary>
/// Change the position of an enemy
/// </summary>
/// <param name="_enemy">Enemy to edit</param>
/// <param name="_gameScreen">Datas bind to the game state</param>
void Enemy_UpdateMovement(Enemy* _enemy, GameScreenData* _gameScreen, Game* game);

/// <summary>
/// Makes an enemy shoot a projectile
/// </summary>
/// <param name="enemy">Shooting enemy</param>
/// <param name="_gameScreen">Datas bind to game state</param>
void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen);

/// <summary>
/// Give a random Int between a min and max given in parameters
/// </summary>
int RandomInt(int min, int max);
