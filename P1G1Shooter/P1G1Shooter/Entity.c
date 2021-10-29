#include "Entity.h"

void Entity_Initialize(Entity* _entity, int _health, int _damages, int _speed)
{
	Entity newEntity;
	newEntity.max_hp = _health;
	newEntity.current_hp = _health;
	newEntity.damages = _damages;
	newEntity.speed = _speed;

	_entity = &newEntity;
}

void Entity_TakeDamage(Entity* _entity, int _damages)
{
	_entity->current_hp -= _damages;

	if (_entity->current_hp < 0)
	{
		_entity->current_hp = 0;
	}
}

void Entity_ReceiveHeal(Entity* _entity, int _heal)
{
	_entity->current_hp += _heal;

	if (_entity->current_hp > _entity->max_hp)
	{
		_entity->current_hp = _entity->max_hp;
	}
}
