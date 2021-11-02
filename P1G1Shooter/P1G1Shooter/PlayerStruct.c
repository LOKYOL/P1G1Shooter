#include "PlayerStruct.h"
#include "projectile.h"

#include "Game.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	memset(newPlayer, 0, sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->entity.displayZone, 5, 0, 3, 2, 1);
	DrawRectangleInDisplayZone
	(
		&newPlayer->entity.displayZone, 
		0, 0, 3, 2, 
		WHITE, BRIGHT_RED, ' '
	);

	Entity_Initialize(&newPlayer->entity, 10, 1, WINDOW_HEIGHT / 3, Player_Update);

	newPlayer->entity.position_x = 5;
}

void Player_Update(void* _player, Game* _game)
{
	Player* myPlayer = (Player*)_player;

	Player_UpdateMovement(myPlayer, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myPlayer->entity.displayZone);

	if (KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		Player_Shoot(myPlayer);
	}
}

void Player_UpdateMovement(Player* _player, Game* _game)
{
	double 
		newpos_x = _player->entity.position_x, 
		newpos_y = _player->entity.position_y;

	if (IsKeyDown(*_game->mInputs, VK_UP) || IsKeyDown(*_game->mInputs, 'Z'))
	{
		newpos_y -= _game->mGameDt * _player->entity.speed;
	}
	if (IsKeyDown(*_game->mInputs, VK_DOWN) || IsKeyDown(*_game->mInputs, 'S'))
	{
		newpos_y += _game->mGameDt * _player->entity.speed;
	}
	/*if (IsKeyDown(*_game->mInputs, VK_LEFT) || IsKeyDown(*_game->mInputs, 'Q'))
	{
		move_x -= _game->mGameDt * _player->entity.speed;
	}*/
	/*if (IsKeyDown(*_game->mInputs, VK_RIGHT) || IsKeyDown(*_game->mInputs, 'D'))
	{
		move_x += _game->mGameDt * _player->entity.speed;
	}*/



	if (newpos_x < 0)
	{
		newpos_x = 0;
	}
	if (newpos_y < 0)
	{
		newpos_y = 0;
	}

	if (newpos_x > WINDOW_WIDTH - _player->entity.displayZone.mSizeX)
	{
		newpos_x = WINDOW_WIDTH - _player->entity.displayZone.mSizeX;
	}
	if (newpos_y > WINDOW_HEIGHT - _player->entity.displayZone.mSizeY)
	{
		newpos_y = WINDOW_HEIGHT - _player->entity.displayZone.mSizeY;
	}

	Entity_MoveTo(&_player->entity, newpos_x, newpos_y);
}

void Player_Shoot(Player* _player)
{
	Projectile newProjectile;
	InitProj(&newProjectile, 2, 0);
	Entity_Initialize(&(newProjectile.projEntity), 10, 1, WINDOW_WIDTH / 6, Projectile_Update);

	
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