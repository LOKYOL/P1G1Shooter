#pragma once

#include "Engine/Game.h"
#include "GameScreen.h" 

typedef struct PowerupHealth
{
	Entity mEntity;
}PowerupHealth;

/// <summary>
/// Initializes a health powerup
/// </summary>
/// <param name="_powerup">Health powerup to initialize</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupHealth_Initialize(PowerupHealth** _powerup, GameScreenData* gameScreen, double posX, double posY);

/// <summary>
/// 
/// </summary>
/// <param name="_powerup">Health powerup to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void PowerupHealth_Update(void* _powerup, Game* _game, GameScreenData* _gameScreen);

/// <summary>
/// Change the position of a health powerup
/// </summary>
/// <param name="_powerup">Health powerup to edit</param>
/// <param name="_gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void PowerupHealth_UpdateMovement(PowerupHealth* _powerup, GameScreenData* _gameScreen, Game* game);
