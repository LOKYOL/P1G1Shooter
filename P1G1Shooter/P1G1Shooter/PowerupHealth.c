#include "PowerupHealth.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"

void PowerupHealth_Initialize(PowerupHealth** _powerup, GameScreenData* _gameScreen, double _posX, double _posY)
{
	PowerupHealth* newPowerup = (PowerupHealth*)malloc(sizeof(PowerupHealth));
	memset(newPowerup, 0, sizeof(PowerupHealth));

	*_powerup = newPowerup;

	Entity_Initialize(&newPowerup->mEntity, TYPE_POWERUP_HEALTH, 
		_posX, _posY, POWERUPHEALTH_HEALTH, POWERUPHEALTH_SPEED,
		&_gameScreen->mSprites[TYPE_POWERUP_HEALTH], 
		PowerupHealth_Update, PowerupHealth_OnCollide, PowerupHealth_Destroy);
}

void PowerupHealth_Update(PowerupHealth* _powerup, Game* _game, GameScreenData* _gameScreen)
{
	PowerupHealth_UpdateMovement(_powerup, _gameScreen, _game);
	FlushDisplayZone(_game->mDisplaySettings, &_powerup->mEntity.mDisplayZone);
}

void PowerupHealth_UpdateMovement(PowerupHealth* _powerup, GameScreenData* _gameScreen, Game* _game)
{
	double newpos_x =
		_powerup->mEntity.mPosition_x -
		(_powerup->mEntity.mSpeed * _game->mGameDt * 10);

	Entity_MoveTo(&_powerup->mEntity, newpos_x, _powerup->mEntity.mPosition_y);

	if (_powerup->mEntity.mPosition_x < -5 || _powerup->mEntity.mPosition_x > WINDOW_WIDTH + 5)
	{
		_powerup->mEntity.mHealth = 0;
	}
}

void PowerupHealth_OnCollide(Entity* _entity)
{
	if (_entity->mEntityType == TYPE_PLAYER)
	{
		Entity_ReceiveHeal(_entity, POWERUPHEALTH_HEAL);
	}
}

void PowerupHealth_Destroy(Entity* _entity)
{
	free(_entity);
}
