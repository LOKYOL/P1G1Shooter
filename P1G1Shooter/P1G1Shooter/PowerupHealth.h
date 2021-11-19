
#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

#define POWERUP_HEALTH_SECTION "PowerUpHealth"

#define POWERUPHEALTH_HEAL 1
#define POWERUPHEALTH_HEALTH	1

typedef struct PowerupHealth
{
	Entity mEntity;
}PowerupHealth;

/// <summary>
/// Initializes a health powerup
/// </summary>
/// <param name="powerup">Health powerup to initialize</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupHealth_Initialize(PowerupHealth** powerup, GameScreenData* gameScreen, double posX, double posY);

/// <summary>
/// Update a health powerup
/// </summary>
/// <param name="powerup">Health powerup to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupHealth_Update(PowerupHealth* powerup, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a health powerup
/// </summary>
/// <param name="powerup">Health powerup to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void PowerupHealth_UpdateMovement(PowerupHealth* powerup, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Check collision of a health powerup
/// </summary>
/// <param name="current">Current powerup</param>
/// <param name="entity">Other entity which collide with the current powerup</param>
/// <param name="game">Current game</param>
void PowerupHealth_OnCollide(PowerupHealth* current, Entity* entity, Game* game);

/// <summary>
/// Free components of a health powerup and destroy him
/// </summary>
/// <param name="entity">Entity casted to health powerup to destroy</param>
void PowerupHealth_Destroy(Entity* entity);
