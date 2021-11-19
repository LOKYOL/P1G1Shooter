#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

#define PROJECTILE_STANDARD_INIT_SECTION "PlayerProjectileStandard"
#define PROJECTILE_AIMASSIST_INIT_SECTION "PlayerProjectileAimAssist"

typedef void (*ProjectileMovement)(struct Projectile*, struct Game*, struct GameScreen*);

typedef struct Projectile
{
	Entity mEntity;

	double mVelocity_x;
	double mVelocity_y;

	ProjectileMovement mMovementUpdate;
}Projectile;

/// <summary>
/// Initializes a projectile
/// </summary>
/// <param name="proj">Projectile to initialize</param>
/// <param name="speed">Speed of the projectile</param>
/// <param name="direction">Direction of the projectile, either left or right</param>
void Proj_Initialize(Projectile** proj, float health,
 float velocity_x,
	float velocity_y,
 double pos_x,
	double pos_y,
 EntityType type, 
	int sprite,
 GameScreenData* gameScreen,
 
	ProjectileMovement movement,

	Update update,
	OnCollide onCollide,

	Destroy destroy
	);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Projectile_Update(Projectile* proj, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Projectile_Movement_Standard(Projectile * proj, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Projectile_Movement_AimAssist(Projectile * proj, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Move the current projectile
/// </summary>
/// <param name="proj">Current projectile</param>
/// <param name="deltaTime">Game deltaTime</param>
void Projectile_Move(Projectile* proj, double deltaTime);

/// <summary>
/// Check collision of a projectile
/// </summary>
/// <param name="current">Current projectile</param>
/// <param name="entity">Other entity which collide with the current </param>
/// <param name="game">Current game</param>
void Projectile_OnCollide(Projectile* current, Entity* entity, Game* game);

/// <summary>
/// Free components of a projectile and destroy him
/// </summary>
/// <param name="entity">Entity casted to projectile to destroy</param>
void Projectile_Destroy(Entity* entity);
