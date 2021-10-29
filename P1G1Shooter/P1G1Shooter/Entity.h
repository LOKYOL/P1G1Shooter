#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef void(*Update)(void* entity, Inputs* inputs, double deltaTime);

typedef struct Entity
{
	DisplayZone	displayZone;

	unsigned int	health;
	// int	max_hp;

	int	damages;

	int	speed;

	Update	Update;
} Entity;

void	Entity_Initialize(Entity* entity, int health, int damages, int speed, Update Update);

unsigned int	GetHealth(Entity* entity);
unsigned int	SetHealth(Entity* entity, unsigned int health);
