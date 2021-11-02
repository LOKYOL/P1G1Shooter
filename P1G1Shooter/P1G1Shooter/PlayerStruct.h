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

void Player_TakeDamage(Player* player, int damages);
void Player_ReceiveHeal(Player* player, int heal);

void Player_Die(Player* player);
