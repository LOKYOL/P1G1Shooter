#include "PowerupHealth.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"

void PowerupHealth_Initialize(PowerupHealth** _powerup, GameScreenData* gameScreen, double posX, double posY)
{
	PowerupHealth* newPowerup = (PowerupHealth*)malloc(sizeof(PowerupHealth));
	memset(newPowerup, 0, sizeof(PowerupHealth));

	*_powerup = newPowerup;

	Entity_Initialize(&newPowerup->mEntity, 1, -1, 40, PowerupHealth_Update);

	newPowerup->mEntity.mDisplayZone = gameScreen->mSprites[TYPE_POWERUP_HEALTH];
	newPowerup->mEntity.mDisplayZone.mPosX = posX;
	newPowerup->mEntity.mDisplayZone.mPosY = posY;

	newPowerup->mEntity.mEntityType = TYPE_POWERUP_HEALTH;
	newPowerup->mEntity.mPosition_x = newPowerup->mEntity.mDisplayZone.mPosX;
	newPowerup->mEntity.mPosition_y = newPowerup->mEntity.mDisplayZone.mPosY;
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
