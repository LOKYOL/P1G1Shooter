#include "PowerupAimAssist.h"

void PowerupAimAssist_Initialize(PowerupAimAssist** _powerUp, GameScreenData* _gameScreen, double _posX, double _posY)
{
	PowerupAimAssist* newPowerUp = (PowerupAimAssist*)malloc(sizeof(PowerupAimAssist));
	memset(newPowerUp, 0, sizeof(PowerupAimAssist));

	*_powerUp = newPowerUp;

	ParamSection* aimAssistSection = GetSection(_gameScreen->mParamsList, POWERUP_AIMASSIST_SECTION);

	if(aimAssistSection)
	{
		ParamInt* aimAssistSpeed = (ParamInt*)GetParamInSection(aimAssistSection, "Speed");

		Entity_Initialize(&newPowerUp->mEntity, TYPE_POWERUP_AIMASSIST,
			_posX, _posY, 1, aimAssistSpeed->mValue,
			&_gameScreen->mSprites[TYPE_POWERUP_AIMASSIST],
			PowerupAimAssist_Update, PowerupAimAssist_OnCollide, PowerupAimAssist_Destroy);
	}
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
		_powerup->mEntity.mCurrentHealth = 0;
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
