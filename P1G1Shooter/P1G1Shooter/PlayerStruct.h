#pragma once

#include "ConsoleDisplay.h"

typedef struct Player
{
	DisplayZone displayZone;

	int	current_hp;
	int	max_hp;

	int	damages;
} Player;

void InitPlayer(Player** player);

void Player_TakeDamage(Player* player, int damages);
void Player_ReceiveHeal(Player* player, int heal);


