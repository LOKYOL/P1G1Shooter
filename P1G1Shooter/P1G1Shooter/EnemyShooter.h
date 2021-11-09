#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct EnemyShooter
{
	Entity mEntity;

	float mShootCooldown;
}EnemyShooter;

void EnemyShooter_Initialize(EnemyShooter** _enemy, unsigned int _health, int _damage, int _speed);

void EnemyShooter_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen);

void EnemyShooter_UpdateMovement(EnemyShooter* _enemy, GameScreenData* _gameScreen, Game* game);

void Enemy_Shoot(EnemyShooter* enemy, GameScreenData* _gameScreen);