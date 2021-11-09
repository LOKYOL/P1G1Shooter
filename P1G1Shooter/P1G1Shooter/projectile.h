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
/// <param name="deltatime">Deltatime of the game</param>
void Projectile_Update(void* _proj, Game* _game, GameScreenData* _gameScreen);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="deltatime">Deltatime of the game</param>
void Projectile_UpdateMovement(Projectile * proj, Game* _game, GameScreenData* _gameScreen);

/// <summary>
/// Destroys a projectile and deallocate its memory
/// </summary>
/// <param name="projectile">Projectile to destroy</param>
void DestroyProjectile(Projectile* projectile);

/// <summary>
/// Check if a value is within a given range
/// </summary>
/// <param name="value">Value to test</param>
/// <param name="min">Minimum value</param>
/// <param name="max">Maximum value</param>
/// <returns></returns>
// int InRange(int value, int min, int max);
