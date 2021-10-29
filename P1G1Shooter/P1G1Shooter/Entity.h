#pragma once

#include "ConsoleDisplay.h"
#include "Inputs.h"

typedef struct Entity
{
	DisplayZone displayZone;

	int	current_hp;
	int	max_hp;

	int	damages;

	int vitesse;
} Entity;

void Entity_Initialize(Entity** entity);
