#include "Entity.h"
#include "Engine/Game.h"
#include "PlayerStruct.h"
#include "Obstacle.h"
#include "projectile.h"

void Entity_Initialize(Entity* _entity, int _health, int _damages, int _speed, EntityUpdate _Update)
{
	_entity->mPosition_x = 0;
	_entity->mPosition_y = 0;
	_entity->mHealth = _health;
	_entity->mDamages = _damages;
	_entity->mSpeed = _speed;

	_entity->mUpdate = _Update;
}

void Entity_Move(Entity* _entity, double _moveX, double _moveY)
{
	_entity->mPosition_x += _moveX * _entity->mSpeed;
	_entity->mPosition_y += _moveY * _entity->mSpeed;

	Entity_UpdateDisplayZone(_entity);
}

void Entity_MoveTo(Entity* _entity, double _posX, double _posY)
{
	_entity->mPosition_x = _posX;
	_entity->mPosition_y = _posY;

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

char Entity_IsDead(Entity* _entity)
{
	return _entity->mHealth <= 0;
}

void Entity_Free(Entity* _entity)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER:
		free(((Player*)_entity));
		break;
	case TYPE_OBSTACLE:
		free(((Obstacle*)_entity));
		break;
	case TYPE_PROJECTILE:
		free(((Projectile*)_entity));
		break;
	default:
		free(_entity);
		break;
	}
}
