#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Projectile
{
	Entity mEntity;

	unsigned char direction; //0 = left, 1 = right
}Projectile;

/// <summary>
/// Initializes a projectile
/// </summary>
/// <param name="proj">Projectile to initialize</param>
/// <param name="speed">Speed of the projectile</param>
/// <param name="direction">Direction of the projectile, either left or right</param>
void Proj_Initialize(Projectile** proj, int speed, int direction, double posPlayer_x, double posPlayer_y, EntityType type, GameScreenData* gameScreen);

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
