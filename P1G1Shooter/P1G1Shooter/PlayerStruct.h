#pragma once

#include "Game.h"

#define PlayerUpdate(player, inputs, deltaTime) \
	Player_Update((void*)player, &inputs, deltaTime)

typedef struct Player
{
	Entity entity;
} Player;

void InitPlayer(Player** player);

void Player_Update(void* player, Game* game);

void Player_UpdateMovement(Player* player, Game* game);

void Player_Shoot(Player* player);
