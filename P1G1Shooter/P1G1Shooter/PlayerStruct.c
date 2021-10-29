#include "PlayerStruct.h"
#include "TimeManagement.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->displayZone, 0, 0, 3, 3, 1);

	newPlayer->current_hp = 10;
	newPlayer->max_hp = 10;

	newPlayer->damages = 1;
}

void Player_UpdateMovement(Player* _player, Inputs _inputs, double _deltaTime)
{
	double move_x = 0, move_y = 0;

	if (IsKeyDown(_inputs, VK_LEFT))
	{
		move_x -= _deltaTime * _player->vitesse;
	}
	if (IsKeyDown(_inputs, VK_UP))
	{
		move_y -= _deltaTime * _player->vitesse;
	}
	if (IsKeyDown(_inputs, VK_RIGHT))
	{
		move_x += _deltaTime * _player->vitesse;
	}
	if (IsKeyDown(_inputs, VK_DOWN))
	{
		move_y += _deltaTime * _player->vitesse;
	}
}

void Player_TakeDamage(Player* _player, int _damages)
{
	_player->current_hp -= _damages;

	if (_player->current_hp < 0)
	{
		_player->current_hp = 0;
		Player_Die(_player);
	}
}

void Player_ReceiveHeal(Player* _player, int _heal)
{
	_player->current_hp += _heal;

	if (_player->current_hp > _player->max_hp)
	{
		_player->current_hp = _player->max_hp;
	}
}

void Player_Die(Player* _player)
{
	// A FAIRE
}