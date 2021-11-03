#include "PlayerStruct.h"
#include "Engine/Game.h"
#include "projectile.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	memset(newPlayer, 0, sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->entity.mDisplayZone, 5, 0, 3, 2, 1);
	DrawRectangleInDisplayZone
	(
		&newPlayer->entity.mDisplayZone, 
		0, 0, 3, 2, 
		WHITE, BRIGHT_RED, ' '
	);

	Entity_Initialize(&newPlayer->entity, 10, 1, WINDOW_HEIGHT / 3, Player_Update);

	newPlayer->entity.mPosition_x = 5;
	newPlayer->entity.mEntityType = TYPE_PLAYER;
}

void Player_Update(void* _player, Game* _game)
{
	Player* myPlayer = (Player*)_player;

	Player_UpdateMovement(myPlayer, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myPlayer->entity.mDisplayZone);

	if (KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		Player_Shoot(myPlayer, _game);
	}
}

void Player_UpdateMovement(Player* _player, Game* _game)
{
	double 
		newpos_x = _player->entity.mPosition_x, 
		newpos_y = _player->entity.mPosition_y;

	if (IsKeyDown(*_game->mInputs, VK_UP) || IsKeyDown(*_game->mInputs, 'Z'))
	{
		newpos_y -= _game->mGameDt * _player->entity.mSpeed;
	}
	if (IsKeyDown(*_game->mInputs, VK_DOWN) || IsKeyDown(*_game->mInputs, 'S'))
	{
		newpos_y += _game->mGameDt * _player->entity.mSpeed;
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

	if (newpos_x > WINDOW_WIDTH - _player->entity.mDisplayZone.mSizeX)
	{
		newpos_x = WINDOW_WIDTH - _player->entity.mDisplayZone.mSizeX;
	}
	if (newpos_y > WINDOW_HEIGHT - _player->entity.mDisplayZone.mSizeY)
	{
		newpos_y = WINDOW_HEIGHT - _player->entity.mDisplayZone.mSizeY;
	}

	Entity_MoveTo(&_player->entity, newpos_x, newpos_y);
}

void Player_Shoot(Player* _player, Game* _game)
{
	Projectile* newProjectile;
	InitProj(&newProjectile, 2, 0, _player->entity.mPosition_x, _player->entity.mPosition_y);
	//Entity_Initialize(&(newProjectile->projEntity), 10, 1, WINDOW_WIDTH / 6, Projectile_Update);

	PushEntity(_game, &newProjectile);
}
