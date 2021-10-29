#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef struct Player
{
	DisplayZone displayZone;

	int	current_hp;
	int	max_hp;

	int	damages;

	int vitesse;
} Player;

void InitPlayer(Player** player);


void Player_UpdateMovement(Player* player, Inputs inputs, double deltaTime);


void Player_TakeDamage(Player* player, int damages);
void Player_ReceiveHeal(Player* player, int heal);

void Player_Die(Player* player);
