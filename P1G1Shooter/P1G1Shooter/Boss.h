#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef enum ContactAttackStatus
{
	NOT_LAUNCHED = 0,
	ATTACKING = 1,
	RETURNING_FROM_ATTACK = 2

}ContactAttackStatus;

typedef struct Boss
{
	Entity mEntity;

	ContactAttackStatus mContactAttackStatus;

	double mShootCooldown;
	double mContactAttackCooldown;
	double mChangeDirectionCooldown;

	double mTargetedPosX;
	double mTargetedPosY;

	int mCurrentDirectionX;
	int mCurrentDirectionY;

}Boss;

/// <summary>
/// Initialize a boss with given parameters
/// </summary>
/// <param name="boss">Boss to initialize</param>
/// <param name="health"> Health of boss</param>
/// <param name="damage">Damage dealt by boss</param>
/// <param name="speed">Speed of boss</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Initialize(Boss** _boss,
	unsigned int _health, int _damage, int _speed,
	GameScreenData* gameScreen);

void Boss_Update(void* _boss, Game* _game, GameScreenData* _gameScreen);

void Boss_UpdateMovement(Boss* _boss, GameScreenData* _gameScreen, Game* game);

void Boss_Shoot(Boss* _boss, GameScreenData* _gameScreen);

void Boss_LaunchContactAttack(Boss* _boss, GameScreenData* _gameScreen);

void Boss_ContactAttack(Boss* _boss, GameScreenData* _gameScreen)