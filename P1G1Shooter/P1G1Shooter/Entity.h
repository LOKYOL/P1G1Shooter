#pragma once

#include "Engine/ConsoleDisplay.h"
#include "Inputs.h"

typedef void(*EntityUpdate)(void* entity, struct Game* game);

typedef struct Entity
{
	DisplayZone		displayZone;

	double			position_x, 
					position_y;

	unsigned int	health;

	int				damages;

	int				speed;

	EntityUpdate	update;
} Entity;

void Entity_Initialize(Entity* entity, int health, int damages, int speed, EntityUpdate Update);

void Entity_Move(Entity* entity, double moveX, double moveY);
void Entity_MoveTo(Entity* entity, double posX, double posY);
void Entity_UpdateDisplayZone(Entity* entity);