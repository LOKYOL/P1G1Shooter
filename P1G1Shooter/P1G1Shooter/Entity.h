#pragma once

#include "Engine/ConsoleDisplay.h"

typedef void(*Update)(void* entity, struct Game*, struct GameScreenData*);
typedef void(*OnCollide)(void* entity, struct Game*);
typedef void(*Destroy)(void* entity);

#define NUM_OF_ENTITY_TYPES 8

typedef enum EntityType
{
	TYPE_ENTITY				= -1,
	TYPE_PLAYER				= 0,
	TYPE_OBSTACLE			= 1,
	TYPE_PLAYER_PROJECTILE	= 2,
	TYPE_ENEMY_PROJECTILE	= 3,
	TYPE_ENEMY				= 4,
	TYPE_ENEMY_KAMIKAZE		= 5,
	TYPE_POWERUP_HEALTH		= 6,
	TYPE_BOSS				= 7
} EntityType;

typedef struct Entity
{
	DisplayZone		mDisplayZone;

	EntityType		mEntityType;

	double			mPosition_x, 
					mPosition_y;

	float			mHealth;

	//int				mDamages;

	float			mSpeed;

	Update			mUpdate;
	OnCollide		mOnCollide;
	Destroy			mDestroy;

} Entity;

/// <summary>
/// Initialize an entity with given parameters
/// </summary>
/// <param name="entity">Entity to initialize</param>
void Entity_Initialize(Entity* entity, EntityType type, int posx, int posy, 
	int health, float speed,
	DisplayZone* displayZone,
	Update update, OnCollide onCollide, Destroy destroy);

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
/// Heal an entity
/// </summary>
/// <param name="entity">Entity to heal</param>
/// <param name="heal">Amount of life recovered</param>
void Entity_ReceiveHeal(Entity* entity, int heal);

/// <summary>
/// Kill the entity by putting his health to 0
/// </summary>
/// <param name="entity">Entity to kill</param>
void Entity_Kill(Entity* entity);

/// <summary>
/// Check if an entity is dead
/// </summary>
/// <param name="entity">Entity to check</param>
char Entity_IsDead(Entity* entity);

/// <summary>
/// Check the distance between 2 entities
/// </summary>
/// <param name="entityA">First entity</param>
/// <param name="entityB">Second entity</param>
/// <returns>The distance between entity A and entity B</returns>
double Entity_GetDistance(Entity* entityA, Entity* entityB);

/// <summary>
/// Makes sure an entity doesn't go out-of-bound on the Y axis
/// </summary>
/// <param name="entity">Entity to check</param>
void Entity_ClampYPosition(Entity* entity);

/// <summary>
/// Makes sure an entity doesn't go out-of-bound on the X axis
/// </summary>
/// <param name="entity">Entity to check</param>
void Entity_ClampXPosition(Entity* entity);

void Entity_Destroy(Entity* entity);
