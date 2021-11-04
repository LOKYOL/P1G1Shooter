#pragma once

#include "Entity.h"
#include "GameScreen.h"

#define MAX_ENERGIE		100
#define SHOOT_COST		10
#define RELOAD_SPEED	25

#define PlayerUpdate(player, inputs, deltaTime) \
	Player_Update((void*)player, &inputs, deltaTime)

typedef struct Player
{
	Entity mEntity;

	float mCurrentEnergie;
	unsigned char mCanShoot;
} Player;

void InitPlayer(Player** player);

void Player_Update(void* player, struct Game* game, GameScreenData* gameScreen);

void Player_UpdateMovement(Player* player, struct Game* game);

void Player_Shoot(Player* player, GameScreenData* _gameScreen);
