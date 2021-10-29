#include "PlayerStruct.h"

#include "Entity.h"
#include "Inputs.h"
#include "projectile.h"
#include "Engine/ConsoleDisplay.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->entity.displayZone, 0, 0, 3, 3, 1);

	Entity_Initialize(&newPlayer->entity, 10, 1, 1, Player_Update);
}

void Player_Update(void* _player, Inputs* _inputs, double _deltaTime) // pyerre top 1 <3
{
	Player* myPlayer = (Player*)_player;

	Player_UpdateMovement(myPlayer, _inputs, _deltaTime);

	if (KeyPressStart(*_inputs, VK_SPACE))
	{
		Player_Shoot(myPlayer);
	}
}

void Player_UpdateMovement(Player* _player, Inputs* _inputs, double _deltaTime)
{
	double 
		move_x = _player->entity.displayZone.mPosX, 
		move_y = _player->entity.displayZone.mPosY;

	if (IsKeyDown(*_inputs, VK_UP) || IsKeyDown(*_inputs, 'Z'))
	{
		move_y -= _deltaTime * _player->entity.speed;
	}
	if (IsKeyDown(*_inputs, VK_DOWN) || IsKeyDown(*_inputs, 'S'))
	{
		move_y += _deltaTime * _player->entity.speed;
	}
	/*if (IsKeyDown(*_inputs, VK_LEFT) || IsKeyDown(*_inputs, 'Q'))
	{
		move_x -= _deltaTime * _player->entity.speed;
	}*/
	/*if (IsKeyDown(*_inputs, VK_RIGHT) || IsKeyDown(*_inputs, 'D'))
	{
		move_x += _deltaTime * _player->entity.speed;
	}*/

	MoveDisplayZone(&_player->entity.displayZone, move_x, move_y);
}

void Player_Shoot(Player* _player)
{
	// A FAIRE
}

void Player_TakeDamage(Player* _player, int _damages)
{
	_player->entity.health -= _damages;
}

void Player_ReceiveHeal(Player* _player, int _heal)
{
	_player->entity.health += _heal;
}

void Player_Die(Player* _player)
{
	// A FAIRE
}