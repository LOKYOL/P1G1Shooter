#include "PowerupAimAssist.h"

void PowerupAimAssist_Initialize(PowerupAimAssist** _powerup, GameScreenData* _gameScreen, double _posX, double _posY)
{
	PowerupAimAssist* newPowerup = (PowerupAimAssist*)malloc(sizeof(PowerupAimAssist));
	memset(newPowerup, 0, sizeof(PowerupAimAssist));

	*_powerup = newPowerup;

	Entity_Initialize(&newPowerup->mEntity, TYPE_POWERUP_AIMASSIST,
		_posX, _posY, 1, POWERUP_AIMASSIST_SPEED,
		&_gameScreen->mSprites[TYPE_POWERUP_AIMASSIST],
		PowerupAimAssist_Update, PowerupAimAssist_OnCollide, PowerupAimAssist_Destroy);
}

void PowerupAimAssist_Update(PowerupAimAssist* _powerup, Game* _game, GameScreenData* _gameScreen)
{
	PowerupHealth_UpdateMovement(_powerup, _gameScreen, _game);
	FlushDisplayZone(_game->mDisplaySettings, &_powerup->mEntity.mDisplayZone);
}

void PowerupAimAssist_UpdateMovement(PowerupAimAssist* _powerup, GameScreenData* _gameScreen, Game* _game)
{
	double newpos_x =
		_powerup->mEntity.mPosition_x -
		(_powerup->mEntity.mSpeed * _game->mGameDt * 10);

	Entity_MoveTo(&_powerup->mEntity, newpos_x, _powerup->mEntity.mPosition_y);

	if (_powerup->mEntity.mPosition_x < -5 || _powerup->mEntity.mPosition_x > (double)(WINDOW_WIDTH + 5))
	{
		_powerup->mEntity.mHealth = 0;
	}
}

void PowerupAimAssist_OnCollide(PowerupAimAssist* _current, Entity* _entity, Game* _game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER:
		Entity_Kill(_current);
		return;
	default:
		return;
	}
}

void PowerupAimAssist_Destroy(Entity* _entity)
{
	free(_entity);
}
