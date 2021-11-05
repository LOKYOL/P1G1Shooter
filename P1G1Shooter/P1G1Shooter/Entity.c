#include <math.h>

#include "Entity.h"
#include "Engine/Game.h"
#include "PlayerStruct.h"
#include "Obstacle.h"
#include "projectile.h"
#include "Enemy.h"

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
	_entity->mPosition_x += _moveX;
	_entity->mPosition_y += _moveY;

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
	case TYPE_PLAYER_PROJECTILE:
	case TYPE_ENEMY_PROJECTILE:
		free(((Projectile*)_entity));
		break;
	case TYPE_ENEMY:
	case TYPE_ENEMY_KAMIKAZE:
		free(((Enemy*)_entity));
		break;
	default:
		free(_entity);
		break;
	}
}

double Entity_GetDistance(Entity* _entityA, Entity* _entityB) {

	if (!(_entityA) || !(_entityB))
		return 0;

	double distance = 0, distanceX = 0, distanceY = 0;

	distanceX = _entityA->mPosition_x - _entityB->mPosition_x;
	distanceY = _entityA->mPosition_y - _entityB->mPosition_y;

	distance = sqrt(pow(distanceX, 2) + pow(distanceY, 2));

	return distance;
}
