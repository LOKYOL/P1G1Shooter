#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef struct Entity
{
	DisplayZone displayZone;

	int	current_hp;
	int	max_hp;

	int	damages;

	int speed;
} Entity;

void Entity_Initialize(Entity* entity, int health, int damages, int speed);

void Entity_TakeDamage(Entity* entity, int damages);
void Entity_ReceiveHeal(Entity* entity, int heal);

void Entity_Die(Entity* entity);
