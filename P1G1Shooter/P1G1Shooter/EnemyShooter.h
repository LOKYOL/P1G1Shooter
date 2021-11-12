#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

#define ENEMYS_HEALTH	1

#define ENEMYK_SPEED_MIN	40
#define ENEMYK_SPEED_MAX	50

typedef struct EnemyShooter
{
	Entity	mEntity;

	double	mShootCooldown;
	double	mChangeDirectionCooldown;

	int		mCurrentDirectionX;
	int		mCurrentDirectionY;
}EnemyShooter;

/// <summary>
/// Initialize an enemy shooter with given parameters
/// </summary>
/// <param name="enemy">Enemy shooter to initialize</param>
/// <param name="health">Health of enemy shooter</param>
/// <param name="damages">Damage dealt by enemy shooter</param>
/// <param name="speed">Speed of enemy shooter</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void EnemyShooter_Initialize(EnemyShooter** enemy, GameScreenData* gameScreen);

/// <summary>
/// Update an enemy shooter
/// </summary>
/// <param name="enemy">Enemy shooter to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void EnemyShooter_Update(void* enemy, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of an enemy shooter
/// </summary>
/// <param name="enemy">Enemy shooter to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void EnemyShooter_UpdateMovement(EnemyShooter* enemy, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Makes the enemy shoot
/// </summary>
/// <param name="enemy">Enemy to update</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Enemy_Shoot(EnemyShooter* enemy, GameScreenData* gameScreen);

void EnemyShooter_OnCollide(Entity* entity);

void EnemyShooter_Destroy(Entity* entity);
