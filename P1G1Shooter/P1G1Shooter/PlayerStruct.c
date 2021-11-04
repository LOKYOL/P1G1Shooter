#include "PlayerStruct.h"
#include "Engine/Game.h"
#include "projectile.h"

void InitPlayer(Player** _player)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	memset(newPlayer, 0, sizeof(Player));
	*_player = newPlayer;

	InitDisplayZone(&newPlayer->mEntity.mDisplayZone, 5, 0, 3, 2, 1);
	DrawRectangleInDisplayZone
	(
		&newPlayer->mEntity.mDisplayZone, 
		0, 0, 3, 2, 
		WHITE, BRIGHT_RED, ' '
	);

	Entity_Initialize(&newPlayer->mEntity, 10, 1, WINDOW_HEIGHT / 3, Player_Update);

	newPlayer->mEntity.mPosition_x = 5;
	newPlayer->mEntity.mEntityType = TYPE_PLAYER;
	newPlayer->mCurrentEnergy = MAX_ENERGY;
	newPlayer->mReloadCooldown = 0.f;
	newPlayer->mShootCooldown = 0.f;
}

void Player_Update(void* _player, Game* _game, GameScreenData* _gameScreen)
{
	Player* myPlayer = (Player*)_player;

	Player_UpdateMovement(myPlayer, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myPlayer->mEntity.mDisplayZone);

	if (KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		Player_Shoot(myPlayer, _gameScreen);
	}
}

void Player_UpdateMovement(Player* _player, Game* _game)
{
	double 
		newpos_x = _player->mEntity.mPosition_x, 
		newpos_y = _player->mEntity.mPosition_y;

	if (IsKeyDown(*_game->mInputs, VK_UP) || IsKeyDown(*_game->mInputs, 'Z'))
	{
		newpos_y -= _game->mGameDt * _player->mEntity.mSpeed;
	}
	if (IsKeyDown(*_game->mInputs, VK_DOWN) || IsKeyDown(*_game->mInputs, 'S'))
	{
		newpos_y += _game->mGameDt * _player->mEntity.mSpeed;
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

	if (newpos_x > WINDOW_WIDTH - _player->mEntity.mDisplayZone.mSizeX)
	{
		newpos_x = WINDOW_WIDTH - _player->mEntity.mDisplayZone.mSizeX;
	}
	if (newpos_y > WINDOW_HEIGHT - _player->mEntity.mDisplayZone.mSizeY)
	{
		newpos_y = WINDOW_HEIGHT - _player->mEntity.mDisplayZone.mSizeY;
	}

	Entity_MoveTo(&_player->mEntity, newpos_x, newpos_y);
}

void Player_Shoot(Player* _player, GameScreenData* _gameScreen)
{
	if (_player->mShootCooldown <= 0)
	{
		Projectile* newProjectile;
		InitProj(&newProjectile, 2, 0, _player->mEntity.mPosition_x, _player->mEntity.mPosition_y);

		PushEntity(_gameScreen, &newProjectile);

		_player->mCurrentEnergy -= SHOOT_COST;
		_player->mReloadCooldown = RELOAD_COOLDOWN;

		if (_player->mCurrentEnergy <= 0)
		{
			_player->mShootCooldown = OVERHEAT_COOLDOWN;
			_player->mCurrentEnergy = 0;
		}
	}
}
