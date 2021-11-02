#pragma once

#include "Entity.h"

typedef struct Projectile 
{
	Entity projEntity;
	/*Vector2 position;
	Vector2 size;*/

	unsigned char direction; //0 = left, 1 = right
}Projectile;

/// <summary>
/// Initializes a projectile
/// </summary>
/// <param name="proj">Projectile to initialize</param>
/// <param name="speed">Speed of the projectile</param>
/// <param name="direction">Direction of the projectile, either left or right</param>
void InitProj(Projectile** proj, int speed, int direction/*, int PosX, int PosY, int SizeX, int SizeY*/);

/// <summary>
/// Change the position of a projectile
/// </summary>
/// <param name="proj">Projectile to edit</param>
/// <param name="deltatime">Deltatime of the game</param>
void UpdateProjMovement(Projectile** proj, int deltatime);

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
int InRange(int value, int min, int max);
