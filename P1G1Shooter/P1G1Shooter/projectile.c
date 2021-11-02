#include <stdio.h>
#include <stdlib.h>
#include "projectile.h"

void InitProj(Projectile** proj, int speed, int direction/*, int PosX, int PosY, int SizeX, int SizeY*/)
{
	(*proj) = (Projectile*)malloc(sizeof(Projectile));
	(*proj)->projEntity = (Entity*)malloc(sizeof(Entity));
	InitDisplayZone(&(*proj)->projEntity->displayZone, 0, 0, 2, 1, 1);
	(*proj)->projEntity->health = 1;
	(*proj)->projEntity->damages = 1;
	(*proj)->projEntity->speed = speed;
	(*proj)->projEntity->update = UpdateProjMovement;
	(*proj)->direction = 1;
	/*(*proj)->position.x = PosX;
	(*proj)->position.y = PosY;
	(*proj)->size.x = SizeX;
	(*proj)->size.y = SizeY;*/
}

void UpdateProjMovement(Projectile** proj, int deltatime)
{
	if ((*proj)->direction)
	{
		(*proj)->projEntity->displayZone.mPosX += (*proj)->projEntity->speed * deltatime;
	}
	else
	{
		(*proj)->projEntity->displayZone.mPosX -= (*proj)->projEntity->speed * deltatime;
	}
}

void DestroyProjectile(Projectile* projectile)
{
	free(projectile->projEntity);
	free(projectile);
}

int InRange(int value, int min, int max)
{
	return (value >= min && value <= max) ? 1 : 0;
}