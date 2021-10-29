#pragma once

typedef struct Player
{
	int	current_hp;
	int	max_hp;

	int	damages;
} Player;

void Player_TakeDamage(Player* player, int damages);
void Player_ReceiveHeal(Player* player, int damages);


