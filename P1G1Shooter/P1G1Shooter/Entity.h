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
	TYPE_ENEMY_KAMIKAZE		= 5,
	TYPE_POWERUP_HEALTH		= 6
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

/// <summary>
/// Initialize an entity with different parameters
/// </summary>
/// <param name="entity">Entity to initialize</param>
/// <param name="health">Health of entity</param>
/// <param name="damages">Damage dealt by entity</param>
/// <param name="speed">Speed of entity</param>
/// <param name="Update">Update function of entity</param>
void Entity_Initialize(Entity* entity, int health, int damages, int speed, EntityUpdate Update);

/// <summary>
/// Moves an entity with xy distance
/// </summary>
/// <param name="entity">Entity to move</param>
/// <param name="moveX">Horizontal distance to move</param>
/// <param name="moveY">Vertical distance to move</param>
void Entity_Move(Entity* entity, double moveX, double moveY);

/// <summary>
/// Moves an entity by setting its XY coordinates
/// </summary>
/// <param name="entity">Entity to move</param>
/// <param name="posX">X position to set</param>
/// <param name="posY">Y position to set</param>
void Entity_MoveTo(Entity* entity, double posX, double posY);

/// <summary>
/// Updates an entity's screen display
/// </summary>
/// <param name="entity">Entity to update</param>
void Entity_UpdateDisplayZone(Entity* entity);

/// <summary>
/// Changes an entity's health
/// </summary>
/// <param name="entity">Entity to update</param>
/// <param name="damages">Damage to take</param>
void Entity_TakeDamages(Entity* entity, int damages);

/// <summary>
/// Check if an entity is dead
/// </summary>
/// <param name="entity"></param>
/// <returns></returns>
char Entity_IsDead(Entity* entity);

double Entity_GetDistance(Entity* entityA, Entity* entityB);

void Entity_ClampYPosition(Entity* entity);

double Entity_GetDistance(Entity* entityA, Entity* entityB);

void Entity_ClampYPosition(Entity* entity);

//void Entity_Free(Entity* entity);

/// <summary>
/// Check the distance between 2 entities
/// </summary>
/// <param name="entityA">First entity</param>
/// <param name="entityB">Second entity</param>
/// <returns>The distance between entity 1 and entity 2</returns>
double Entity_GetDistance(Entity* entityA, Entity* entityB);

/// <summary>
/// Makes sure an entity doesn't go out-of-bound on the Y axis
/// </summary>
/// <param name="entity">Entity to check</param>
void Entity_ClampYPosition(Entity* entity);
