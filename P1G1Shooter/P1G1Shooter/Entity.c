#include "Entity.h"

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
