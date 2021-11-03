#pragma once

#include "Engine/ConsoleDisplay.h"
#include "Inputs.h"

typedef void(*EntityUpdate)(void* entity, struct Game* game);

typedef enum EntityType
{
	ENTITY		= 0,
	PLAYER		= 1,
	OBSTACLE	= 2,
	PROJECTILE	= 3,
} EntityType;

typedef struct Entity
{
	DisplayZone		mDisplayZone;

	EntityType		mEntityType;

	double			mPosition_x, 
					mPosition_y;

	unsigned int	mHealth;

	int				mDamages;

	int				mSpeed;

	EntityUpdate	mUpdate;
} Entity;

void Entity_Initialize(Entity* entity, int health, int damages, int speed, EntityUpdate Update);

void Entity_Move(Entity* entity, double moveX, double moveY);
void Entity_MoveTo(Entity* entity, double posX, double posY);
void Entity_UpdateDisplayZone(Entity* entity);


