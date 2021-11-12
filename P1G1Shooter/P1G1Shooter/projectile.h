#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Projectile
{
	Entity mEntity;

	float mVelocity_x;
	float mVelocity_y;
}Projectile;

/// <summary>
/// Initializes a projectile
/// </summary>
/// <param name="proj">Projectile to initialize</param>
/// <param name="speed">Speed of the projectile</param>
/// <param name="direction">Direction of the projectile, either left or right</param>
void Proj_Initialize(Projectile** proj, float speed, int health,
	float velocity_x, float velocity_y,
	double pos_x, double pos_y, 
	EntityType type, GameScreenData* gameScreen,
	Update update, // = Projectile_Update, 
	OnCollide onCollide, // = Projectile_OnCollide, 
	Destroy destroy // = Projectile_Destroy
	);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Projectile_Update(void* proj, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Projectile_UpdateMovement(Projectile * proj, Game* game, GameScreenData* gameScreen);

void Projectile_OnCollide(Entity* entity, Game* game);

void Projectile_Destroy(Entity* entity);