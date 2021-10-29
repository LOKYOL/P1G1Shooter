#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"
#include "Entity.h"

typedef struct Player
{
	Entity entity;
} Player;

void InitPlayer(Player** player);

void Player_Update(void* player, Inputs* inputs, double deltaTime);

void Player_UpdateMovement(Player* player, Inputs* inputs, double deltaTime);

void Player_Shoot(Player* player);

void Player_TakeDamage(Player* player, int damages);
void Player_ReceiveHeal(Player* player, int heal);

void Player_Die(Player* player);
