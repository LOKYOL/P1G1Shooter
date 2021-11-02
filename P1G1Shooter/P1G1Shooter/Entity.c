#include "Entity.h"

void Entity_Initialize(Entity* _entity, int _health, int _damages, int _speed, EntityUpdate _Update)
{
	_entity->position_x = 0;
	_entity->position_y = 0;
	_entity->health = _health;
	_entity->damages = _damages;
	_entity->speed = _speed;

	_entity->update = _Update;
}

void Entity_Move(Entity* _entity, double _moveX, double _moveY)
{
	_entity->position_x += _moveX * _entity->speed;
	_entity->position_y += _moveY * _entity->speed;

	Entity_UpdateDisplayZone(_entity);
}

void Entity_MoveTo(Entity* _entity, double _posX, double _posY)
{
	_entity->position_x = _posX;
	_entity->position_y = _posY;

	Entity_UpdateDisplayZone(_entity);
}

void Entity_UpdateDisplayZone(Entity* _entity)
{
	MoveDisplayZone(&_entity->displayZone, _entity->position_x, _entity->position_y);
}
