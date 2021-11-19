#include "PowerupHealth.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"

void PowerupHealth_Initialize(PowerupHealth** _powerUp, GameScreenData* _gameScreen, double _posX, double _posY)
{
	PowerupHealth* newPowerUp = (PowerupHealth*)malloc(sizeof(PowerupHealth));
	memset(newPowerUp, 0, sizeof(PowerupHealth));

	*_powerUp = newPowerUp;

	ParamSection* healthSection = GetSection(_gameScreen->mParamsList, POWERUP_HEALTH_SECTION);

	if(healthSection)
	{
		ParamInt* healthSpeed = (ParamInt*)GetParamInSection(healthSection, "Speed");

		Entity_Initialize(&newPowerUp->mEntity, TYPE_POWERUP_HEALTH, 
		_posX, _posY, POWERUPHEALTH_HEALTH, healthSpeed->mValue,
		&_gameScreen->mSprites[TYPE_POWERUP_HEALTH], 
		PowerupHealth_Update, PowerupHealth_OnCollide, PowerupHealth_Destroy);
	}
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

	if (_powerup->mEntity.mPosition_x < -5 || _powerup->mEntity.mPosition_x > (long long)WINDOW_WIDTH + 5)
	{
		_powerup->mEntity.mCurrentHealth = 0;
	}
}

void PowerupHealth_OnCollide(PowerupHealth* _current, Entity* _entity, Game* game)
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

void PowerupHealth_Destroy(Entity* _entity)
{
	free(_entity);
}
