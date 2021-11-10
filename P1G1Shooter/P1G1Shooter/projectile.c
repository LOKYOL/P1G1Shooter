#include "projectile.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"

void Proj_Initialize(Projectile** _proj, int speed, int direction, double posPlayer_x, double posPlayer_y, EntityType _type, GameScreenData* _gameScreen)
{
	Projectile* newProjectile = (Projectile*)malloc(sizeof(Projectile));
	memset(newProjectile, 0, sizeof(Projectile));

	*_proj = newProjectile;

	Entity_Initialize(&(*_proj)->mEntity, 1, 1, WINDOW_WIDTH / 4, Projectile_Update);

	newProjectile->mEntity.mDisplayZone = _gameScreen->mSprites[_type];

	newProjectile->mEntity.mPosition_x = (posPlayer_x + 8);
	newProjectile->mEntity.mPosition_y = posPlayer_y;
	newProjectile->direction = direction;
	newProjectile->mEntity.mEntityType = _type;
}

void Projectile_Update(void* _proj, Game* _game, GameScreenData* _gameScreen) {

	Projectile* myProjectile = (Projectile*)_proj;

	Projectile_UpdateMovement(myProjectile, _game, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &myProjectile->mEntity.mDisplayZone);
}

void Projectile_UpdateMovement(Projectile * _proj, Game* _game, GameScreenData* _gameScreen)
{
	double newpos_x = _proj->mEntity.mPosition_x, newpos_y = _proj->mEntity.mPosition_y;

	if (_proj->direction)
	{
		newpos_x += _proj->mEntity.mSpeed * _game->mGameDt;
	}
	else
	{
		newpos_x -= _proj->mEntity.mSpeed * _game->mGameDt;
	}

	Entity_MoveTo(&_proj->mEntity, newpos_x, newpos_y);

	if (_proj->mEntity.mPosition_x < -5 || _proj->mEntity.mPosition_x > WINDOW_WIDTH + 5)
	{
		_proj->mEntity.mHealth = 0;
	}
}
