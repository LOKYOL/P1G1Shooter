#include "Entity.h"

void Entity_Initialize(Entity** _entity)
{
	Entity* newEntity = (Entity*)malloc(sizeof(Entity));
	*_entity = newEntity;
}
