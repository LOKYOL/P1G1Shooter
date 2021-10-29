#include "PlayerStruct.h"
#include "TimeManagement.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->entity.displayZone, 0, 0, 3, 3, 1);

	Entity_Initialize(&newPlayer->entity, 10, 1, 1);
}

void Player_UpdateMovement(Player* _player, Inputs _inputs, double _deltaTime)
{
	double move_x = 0, move_y = 0;

	if (IsKeyDown(_inputs, VK_LEFT))
	{
		move_x -= _deltaTime * _player->entity.speed;
	}
	if (IsKeyDown(_inputs, VK_UP))
	{
		move_y -= _deltaTime * _player->entity.speed;
	}
	if (IsKeyDown(_inputs, VK_RIGHT))
	{
		move_x += _deltaTime * _player->entity.speed;
	}
	if (IsKeyDown(_inputs, VK_DOWN))
	{
		move_y += _deltaTime * _player->entity.speed;
	}
}

void Player_TakeDamage(Player* _player, int _damages)
{
	Entity_TakeDamage(&_player->entity, _damages);
}

void Player_ReceiveHeal(Player* _player, int _heal)
{
	Entity_ReceiveHeal(&_player->entity, _heal);
}

void Player_Die(Player* _player)
{
	// A FAIRE
}