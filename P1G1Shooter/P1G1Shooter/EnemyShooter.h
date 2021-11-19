#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

#define ENEMYS_INIT_SECTION "EnemyShooter"

#define ENEMY_SHOOTER_DIRECTION_COOLDOWN	1

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

/// <summary>
/// Check collision of an ennemy shooter
/// </summary>
/// <param name="current">Current ennemy shooter</param>
/// <param name="entity">Other entity which collide with the current</param>
/// <param name="game">Current game</param>
void EnemyShooter_OnCollide(EnemyShooter* current, Entity* entity, Game* game);

/// <summary>
/// Free components of an enemy shooter and destroy him
/// </summary>
/// <param name="entity">Entity casted to an enemy shooter to destroy</param>
void EnemyShooter_Destroy(Entity* entity);
