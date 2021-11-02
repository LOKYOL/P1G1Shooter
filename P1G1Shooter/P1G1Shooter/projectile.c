#include <stdio.h>
#include <stdlib.h>
#include "projectile.h"

void InitProj(Projectile** proj, int speed, int direction)
{
	(*proj) = (Projectile*)malloc(sizeof(Projectile));
	InitDisplayZone(&(*proj)->projEntity.displayZone, 0, 0, 2, 1, 1);
	(*proj)->direction = 1;
}

void Projectile_Update(Projectile ** _proj, Game* _game) {
	Projectile* myProjectile = (Projectile*)_proj;

	Projectile_UpdateMovement(myProjectile, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myProjectile->projEntity.displayZone);

	if (KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		Player_Shoot(myProjectile);
	}
}

void Projectile_UpdateMovement(Projectile** proj, Game* _game)
{
	if ((*proj)->direction)
	{
		(*proj)->projEntity.displayZone.mPosX += (*proj)->projEntity.speed * _game->mGameDt;
	}
	else
	{
		(*proj)->projEntity.displayZone.mPosX -= (*proj)->projEntity.speed * _game->mGameDt;
	}
}

void DestroyProjectile(Projectile* projectile)
{
	free(projectile);
}

int InRange(int value, int min, int max)
{
	return (value >= min && value <= max) ? 1 : 0;
}