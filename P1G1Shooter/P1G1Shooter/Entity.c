#include "Entity.h"

void Entity_Initialize(Entity* _entity, int _health, int _damages, int _speed, Move _Move)
{
	Entity newEntity;
	//newEntity.max_hp = _health;
	newEntity.health = _health;
	newEntity.damages = _damages;
	newEntity.speed = _speed;

	newEntity.Move = _Move;

	_entity = &newEntity;
}

unsigned int GetHealth(Entity* _entity)
{
	return _entity->health;
}

unsigned int SetHealth(Entity* _entity, unsigned int _health)
{
	return (_entity->health = _health);
}
