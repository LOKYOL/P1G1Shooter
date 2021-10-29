#include "PlayerStruct.h"
#include "TimeManagement.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->entity.displayZone, 0, 0, 3, 3, 1);

	newPlayer->entity.current_hp = 10;
	newPlayer->entity.max_hp = 10;

	newPlayer->entity.damages = 1;
}

void Player_UpdateMovement(Player* _player, Inputs _inputs, double _deltaTime)
{
	double move_x = 0, move_y = 0;

	if (IsKeyDown(_inputs, VK_LEFT))
	{
		move_x -= _deltaTime * _player->entity.vitesse;
	}
	if (IsKeyDown(_inputs, VK_UP))
	{
		move_y -= _deltaTime * _player->entity.vitesse;
	}
	if (IsKeyDown(_inputs, VK_RIGHT))
	{
		move_x += _deltaTime * _player->entity.vitesse;
	}
	if (IsKeyDown(_inputs, VK_DOWN))
	{
		move_y += _deltaTime * _player->entity.vitesse;
	}
}

void Player_TakeDamage(Player* _player, int _damages)
{
	_player->entity.current_hp -= _damages;

	if (_player->entity.current_hp < 0)
	{
		_player->entity.current_hp = 0;
		Player_Die(_player);
	}
}

void Player_ReceiveHeal(Player* _player, int _heal)
{
	_player->entity.current_hp += _heal;

	if (_player->entity.current_hp > _player->entity.max_hp)
	{
		_player->entity.current_hp = _player->entity.max_hp;
	}
}

void Player_Die(Player* _player)
{
	// A FAIRE
}
