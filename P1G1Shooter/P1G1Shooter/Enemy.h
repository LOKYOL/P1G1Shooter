#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

typedef struct Enemy
{
	Entity mEntity;
}Enemy;

/// <summary>
/// Initializes an enemy
/// </summary>
/// <param name="enemy">Enemy to initialize</param>
void InitEnemy(Enemy** _enemy, unsigned int _health, int _damage, int _speed);

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
void Enemy_UpdateMovement(Enemy* _enemy, GameScreenData* _gameScreen, double deltaTime);

/// <summary>
/// 
/// </summary>
/// <param name="enemy"></param>
/// <param name="_gameScreen"></param>
void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen);