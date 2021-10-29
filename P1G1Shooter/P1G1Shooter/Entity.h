#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef void(*EntityUpdate)(void* entity, Inputs* inputs, double deltaTime);

typedef struct Entity
{
	DisplayZone	displayZone;

	unsigned int	health;
	// int	max_hp;

	int	damages;

	int	speed;

	EntityUpdate	update;
} Entity;

void Entity_Initialize(Entity* entity, int health, int damages, int speed, EntityUpdate Update);
