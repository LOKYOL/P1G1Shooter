#pragma once

#include "Engine/ConsoleDisplay.h"

typedef void(*EntityUpdate)(void* entity, struct Game* game, struct GameScreenData* gameScreen);

typedef enum EntityType
{
	TYPE_ENTITY				= -1,
	TYPE_PLAYER				= 0,
	TYPE_OBSTACLE			= 1,
	TYPE_PLAYER_PROJECTILE	= 2,
	TYPE_ENEMY_PROJECTILE	= 3,
	TYPE_ENEMY				= 4,
	TYPE_ENEMY_KAMIKAZE		= 5
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

void Entity_TakeDamages(Entity* entity, int damages);

char Entity_IsDead(Entity* entity);

//void Entity_Free(Entity* entity);

double Entity_GetDistance(Entity* entityA, Entity* entityB);

void Entity_ClampYPosition(Entity* entity);