#include <math.h>

#include "Entity.h"
#include "Engine/Game.h"
#include "PlayerStruct.h"
#include "Obstacle.h"
#include "projectile.h"
#include "Enemy.h"
#include "Engine/Game.h"

void Entity_Initialize(Entity* _entity, EntityType _type, int _posx, int _posy, 
	float _health, float _speed,
	DisplayZone* _displayZone,
	Update _update, OnCollide _onCollide, Destroy _destroy)
{
	_entity->mDisplayZone = *_displayZone;
	_entity->mDisplayZone.mPosX = _posx;
	_entity->mDisplayZone.mPosY = _posy;

	_entity->mEntityType = _type;

	_entity->mPosition_x = _posx;
	_entity->mPosition_y = _posy;
	_entity->mHealth = _health;
	_entity->mSpeed = _speed;
	_entity->mUpdate = _update;
	_entity->mOnCollide = _onCollide;
	_entity->mDestroy = _destroy;
}

void Entity_Move(Entity* _entity, double _moveX, double _moveY)
{
	_entity->mPosition_x += _moveX;
	_entity->mPosition_y += _moveY;

	Entity_ClampYPosition(_entity);

	Entity_UpdateDisplayZone(_entity);
}

void Entity_MoveTo(Entity* _entity, double _posX, double _posY)
{
	_entity->mPosition_x = _posX;
	_entity->mPosition_y = _posY;

	Entity_ClampYPosition(_entity);

	Entity_UpdateDisplayZone(_entity);
}

void Entity_UpdateDisplayZone(Entity* _entity)
{
	MoveDisplayZone(&_entity->mDisplayZone, _entity->mPosition_x, _entity->mPosition_y);
}

void Entity_TakeDamages(Entity* _entity, int _damages)
{
	if (_entity->mHealth < _damages)
	{
		_entity->mHealth = 0;
	}
	else
	{
		_entity->mHealth -= _damages;
	}
}

void Entity_ReceiveHeal(Entity* _entity, int _heal)
{
	_entity->mHealth += _heal;

	if (_entity->mHealth > 3)
	{
		_entity->mHealth = 3;
	}
}

void Entity_Kill(Entity* _entity)
{
	_entity->mHealth = 0.f;
}

char Entity_IsDead(Entity* _entity)
{
	return _entity->mHealth < 1;
}

double Entity_GetDistance(Entity* _entityA, Entity* _entityB) {

	if (!(_entityA) || !(_entityB))
		return 0;
	double
		posXA = _entityA->mPosition_x + ((double)_entityA->mDisplayZone.mSizeX / 2), // Center ?
		posXB = _entityB->mPosition_x + ((double)_entityB->mDisplayZone.mSizeX / 2),
		posYA = _entityA->mPosition_y + ((double)_entityA->mDisplayZone.mSizeY / 2),
		posYB = _entityB->mPosition_y + ((double)_entityB->mDisplayZone.mSizeY / 2);

	return sqrt(pow(posXB - posXA, 2) + pow(posYB - posYA, 2));
}

void Entity_ClampYPosition(Entity* _entity)
{
	if (_entity->mPosition_y < 0)
		_entity->mPosition_y = 0;
	if (_entity->mPosition_y > WINDOW_HEIGHT - _entity->mDisplayZone.mSizeY)
		_entity->mPosition_y = WINDOW_HEIGHT - _entity->mDisplayZone.mSizeY;
}

void Entity_ClampXPosition(Entity* _entity)
{
	if (_entity->mPosition_x < 0)
		_entity->mPosition_x = 0;
	if (_entity->mPosition_x > WINDOW_HEIGHT - _entity->mDisplayZone.mSizeX)
		_entity->mPosition_x = WINDOW_HEIGHT - _entity->mDisplayZone.mSizeX;
}

void Entity_Destroy(Entity* _entity)
{
	free(_entity);
}
