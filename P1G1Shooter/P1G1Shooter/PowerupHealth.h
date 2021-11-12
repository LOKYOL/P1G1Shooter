#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

#define POWERUPHEALTH_HEALTH	1
#define POWERUPHEALTH_SPEED		2
#define POWERUPHEALTH_HEAL		1

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

void PowerupHealth_OnCollide(Entity* entity, Game* game);

void PowerupHealth_Destroy(Entity* entity);
