#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef void(*Move)(Entity* entity);

typedef struct Entity
{
	DisplayZone	displayZone;

	unsigned int	health;
	// int	max_hp;

	int	damages;

	int	speed;

	Move	Move;
} Entity;

void	Entity_Initialize(Entity* entity, int health, int damages, int speed, Move Move);

unsigned int	GetHealth(Entity* entity);
unsigned int	SetHealth(Entity* entity, unsigned int health);
