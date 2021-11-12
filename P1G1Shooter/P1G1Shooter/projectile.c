#include "Projectile.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"

void Proj_Initialize(Projectile** _proj, float _speed, int _health,
	float _velocity_x, float _velocity_y,
	double _pos_x, double _pos_y,
	EntityType _type, GameScreenData* _gameScreen,
	Update _update, OnCollide _onCollide, Destroy _destroy)
{
	Projectile* newProjectile = (Projectile*)malloc(sizeof(Projectile));
	memset(newProjectile, 0, sizeof(Projectile));

	*_proj = newProjectile;

	Entity_Initialize(newProjectile, _type, _pos_x, _pos_y, _health, _speed,
		&_gameScreen->mSprites[_type],
		_update, _onCollide, _destroy);

	newProjectile->mVelocity_x = _velocity_x;
	newProjectile->mVelocity_y = _velocity_y;
}

void Projectile_Update(void* _proj, Game* _game, GameScreenData* _gameScreen) {

	Projectile* myProjectile = (Projectile*)_proj;

	Projectile_UpdateMovement(myProjectile, _game, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &myProjectile->mEntity.mDisplayZone);
}

void Projectile_UpdateMovement(Projectile * _proj, Game* _game, GameScreenData* _gameScreen)
{
	double move_x = 0, move_y = 0;

	move_x += (double)_proj->mVelocity_x * (double)_proj->mEntity.mSpeed * _game->mGameDt;
	move_y += (double)_proj->mVelocity_y * (double)_proj->mEntity.mSpeed * _game->mGameDt;

	Entity_Move(&_proj->mEntity, move_x, move_y);

	if (_proj->mEntity.mPosition_x < -5 || _proj->mEntity.mPosition_x > WINDOW_WIDTH + 5)
	{
		_proj->mEntity.mHealth = 0;
	}
}

void Projectile_OnCollide(Entity* _entity)
{
	EntityType type = _entity->mEntityType;
	if (type == TYPE_PLAYER || type == TYPE_ENEMY_KAMIKAZE || type == TYPE_OBSTACLE)
	{
		Entity_TakeDamages(_entity, 1);
	}
	else if (type == TYPE_PLAYER_PROJECTILE || type == TYPE_ENEMY_PROJECTILE)
	{
		Entity_Kill(_entity);
	}
}

void Projectile_Destroy(Entity* _entity)
{
	free(_entity);
}
