#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Boss
{
	Entity mEntity;

	double mShootCooldown;
	double mChangeDirectionCooldown;

	int mCurrentDirectionX;
	int mCurrentDirectionY;
}Boss;

void Boss_Initialize(Boss** _boss,
	unsigned int _health, int _damage, int _speed,
	GameScreenData* gameScreen);

void Boss_Update(void* _boss, Game* _game, GameScreenData* _gameScreen);

void Boss_UpdateMovement(Boss* _boss, GameScreenData* _gameScreen, Game* game);

// boss shooting projectiles ?
void Boss_Shoot(Boss* _boss, GameScreenData* _gameScreen);