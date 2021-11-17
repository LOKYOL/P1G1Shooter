#include "Projectile.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"
#include <math.h>

void Proj_Initialize(Projectile** _proj, float _speed, float _health,
	float _velocity_x, float _velocity_y,
	double _pos_x, double _pos_y,
	EntityType _type, GameScreenData* _gameScreen,
	ProjectileMovement _movement, Update _update, OnCollide _onCollide, Destroy _destroy)
{
	Projectile* newProjectile = (Projectile*)malloc(sizeof(Projectile));
	memset(newProjectile, 0, sizeof(Projectile));

	*_proj = newProjectile;

	Entity_Initialize((Entity*)newProjectile, _type, _pos_x, _pos_y, _health, _speed,
		&_gameScreen->mSprites[_type],
		_update, _onCollide, _destroy);

	newProjectile->mVelocity_x = _velocity_x;
	newProjectile->mVelocity_y = _velocity_y;

	newProjectile->mMovementUpdate = _movement;
}

void Projectile_Update(Projectile* _proj, Game* _game, GameScreenData* _gameScreen) 
{
	_proj->mMovementUpdate(_proj, _game, _gameScreen);

	FlushDisplayZone(_game->mDisplaySettings, &_proj->mEntity.mDisplayZone);
}

void Projectile_Movement_Standard(Projectile * _proj, Game* _game, GameScreenData* _gameScreen)
{
	Projectile_Move(_proj, _game->mGameDt);

	if (_proj->mEntity.mPosition_x < -5 || _proj->mEntity.mPosition_x > (double)(WINDOW_WIDTH + 5))
	{
		_proj->mEntity.mHealth = 0;
	}
}

void Projectile_Movement_AimAssist(Projectile * _proj, Game* _game, GameScreenData* _gameScreen)
{
	double minDistance = AIM_ASSIST_RANGE;
	Entity* minEntity = NULL;
	double curDistance = 0;

	EntityType curType = 0;

	Entity* curEntity = NULL;
	for (int i = 0; i < _gameScreen->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(_gameScreen->mAllEntities, Entity*, i);
		curType = curEntity->mEntityType;
		if ((curType == TYPE_ENEMY_SHOOTER || curType == TYPE_ENEMY_KAMIKAZE || 
			curType == TYPE_ENEMY_BOSS)
			&& 
			(curDistance = Entity_GetDistance(_proj, curEntity)) < minDistance)
		{
			minDistance = curDistance;
			minEntity = curEntity;
		}
	}

	if (minEntity)
	{
		double 
			targetVelocityX = Entity_GetXPosition(minEntity) - Entity_GetXPosition(_proj),
			targetVelocityY = Entity_GetYPosition(minEntity) - Entity_GetYPosition(_proj);


		if (minDistance <= ULTRA_AIM_ASSIST_RANGE)
		{
			_proj->mVelocity_x = targetVelocityX;
			_proj->mVelocity_y = targetVelocityY;
		}
		else
		{
			double targetMagnitude = minDistance;

			targetVelocityX /= targetMagnitude * targetMagnitude;
			targetVelocityY /= targetMagnitude * targetMagnitude;


			_proj->mVelocity_x += targetVelocityX * _game->mGameDt * AIM_ASSIST_RANGE;
			_proj->mVelocity_y += targetVelocityY * _game->mGameDt * AIM_ASSIST_RANGE;

			targetMagnitude =
				sqrt((_proj->mVelocity_x * _proj->mVelocity_x) +
					(_proj->mVelocity_y * _proj->mVelocity_y));

			_proj->mVelocity_x /= targetMagnitude;
			_proj->mVelocity_y /= targetMagnitude;
		}
	}

	Projectile_Move(_proj, _game->mGameDt);

	if (_proj->mEntity.mPosition_x < -5 || _proj->mEntity.mPosition_x > (double)(WINDOW_WIDTH + 5))
	{
		Entity_Kill(_proj);
	}
}

void Projectile_Move(Projectile* _proj, double _deltaTime)
{
	Entity_Move(_proj,
		_proj->mVelocity_x * _proj->mEntity.mSpeed * _deltaTime,
		_proj->mVelocity_y * _proj->mEntity.mSpeed * _deltaTime);
}

void Projectile_OnCollide(Projectile* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER:
	case TYPE_OBSTACLE:
	case TYPE_PLAYER_PROJECTILE:
	case TYPE_ENEMY_KAMIKAZE:
		Entity_Kill((Entity*)_current);
		return;
	default:
		return;
	}
}

void Projectile_Destroy(Entity* _entity)
{
	free(_entity);
}
