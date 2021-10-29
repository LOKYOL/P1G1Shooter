#include "Entity.h"

void Entity_Initialize(Entity* _entity, int _health, int _damages, int _speed, EntityUpdate _Update)
{
	Entity newEntity;
	//newEntity.max_hp = _health;
	newEntity.health = _health;
	newEntity.damages = _damages;
	newEntity.speed = _speed;

	newEntity.update = _Update;

	_entity = &newEntity;
}
