#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

#define POWERUP_AIMASSIST_DELAY		8.0
#define POWERUP_AIMASSIST_SPEED		2

typedef struct PowerupAimAssist
{
	Entity mEntity;
}PowerupAimAssist;

/// <summary>
/// Initializes a health powerup
/// </summary>
/// <param name="powerup">Health powerup to initialize</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupAimAssist_Initialize(PowerupAimAssist** powerup, GameScreenData* gameScreen, double posX, double posY);

/// <summary>
/// Update a health powerup
/// </summary>
/// <param name="powerup">Health powerup to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupAimAssist_Update(PowerupAimAssist* powerup, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a health powerup
/// </summary>
/// <param name="powerup">Health powerup to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void PowerupAimAssist_UpdateMovement(PowerupAimAssist* powerup, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Check collision of a health powerup
/// </summary>
/// <param name="current">Current powerup</param>
/// <param name="entity">Other entity which collide with the current powerup</param>
/// <param name="game">Current game</param>
void PowerupAimAssist_OnCollide(PowerupAimAssist* current, Entity* entity, Game* game);

/// <summary>
/// Free components of a health powerup and destroy him
/// </summary>
/// <param name="entity">Entity casted to health powerup to destroy</param>
void PowerupAimAssist_Destroy(Entity* entity);