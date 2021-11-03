#include <stdio.h>
#include <stdlib.h>
#include "projectile.h"

void InitProj(Projectile** proj, int speed, int direction, double posPlayer_x, double posPlayer_y)
{
	(*proj) = (Projectile*)malloc(sizeof(Projectile));
	InitDisplayZone(&(*proj)->projEntity.mDisplayZone, (posPlayer_x + 3), posPlayer_y, 2, 1, 1);

	DrawRectangleInDisplayZone
	(
		&(*proj)->projEntity.mDisplayZone,
		0, 0, 2, 1,
		WHITE, RED, ' '
	);

	Entity_Initialize(&(*proj)->projEntity, 1, 1, WINDOW_WIDTH / 4, Projectile_Update);
	(*proj)->projEntity.mPosition_x = (posPlayer_x + 3);
	(*proj)->projEntity.mPosition_y = posPlayer_y;
	(*proj)->direction = 1;
}

void Projectile_Update(void* _proj, Game* _game) {

	Projectile* myProjectile = (Projectile*)_proj;

	Projectile_UpdateMovement(myProjectile, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myProjectile->projEntity.mDisplayZone);
}

void Projectile_UpdateMovement(Projectile * proj, Game* _game)
{
	double newpos_x = proj->projEntity.mPosition_x, newpos_y = proj->projEntity.mPosition_y;

		if (proj->direction)
		{
			newpos_x += proj->projEntity.mSpeed * _game->mGameDt;
		}
		else
		{
			newpos_y -= proj->projEntity.mSpeed * _game->mGameDt;
		}

		Entity_MoveTo(&proj->projEntity, newpos_x, newpos_y);
}

void DestroyProjectile(Projectile* projectile)
{
	free(projectile);
}

int InRange(int value, int min, int max)
{
	return (value >= min && value <= max) ? 1 : 0;
}