#pragma once

#include "Entity.h"

#define PlayerUpdate(player, inputs, deltaTime) \
	Player_Update((void*)player, &inputs, deltaTime)

typedef struct Player
{
	Entity entity;
} Player;

void InitPlayer(Player** player);

void Player_Update(void* player, struct Game* game);

void Player_UpdateMovement(Player* player, struct Game* game);

void Player_Shoot(Player* player, struct Game* _game);
