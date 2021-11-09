#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct EnemyShooter
{
	Entity mEntity;

	double mShootCooldown;
	double mChangeDirectionCooldown;

	int mCurrentDirectionX;
	int mCurrentDirectionY;
}EnemyShooter;

void EnemyShooter_Initialize(EnemyShooter** _enemy, 
	unsigned int _health, int _damage, int _speed,
	GameScreenData* gameScreen);

void EnemyShooter_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen);

void EnemyShooter_UpdateMovement(EnemyShooter* _enemy, GameScreenData* _gameScreen, Game* game);

void Enemy_Shoot(EnemyShooter* enemy, GameScreenData* _gameScreen);