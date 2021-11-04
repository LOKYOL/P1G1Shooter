#pragma once

#include "Game.h"
#include "GameScreen.h" 

typedef struct Enemy
{
	Entity mEntity;
}Enemy;

/// <summary>
/// Initializes an enemy
/// </summary>
/// <param name="enemy">Enemy to initialize</param>
void InitEnemy(Enemy** enemy);

/// <summary>
/// Change the position of an enemy
/// </summary>
/// <param name="enemy">Enemy to edit</param>
/// <param name="game"></param>
/// <param name="gameScreen"></param>
void Enemy_Update(void* _enemy, struct Game* _game, GameScreenData* _gameScreen);

/// <summary>
/// Change the position of an enemy
/// </summary>
/// <param name="enemy">Enemy to edit</param>
/// <param name="game"></param>
void Enemy_UpdateMovement(Enemy* _enemy, struct Game* _game/*, Player* player*/);

/// <summary>
/// 
/// </summary>
/// <param name="enemy"></param>
/// <param name="_gameScreen"></param>
void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen);

void Enemy_Destroy(Enemy* enemy);