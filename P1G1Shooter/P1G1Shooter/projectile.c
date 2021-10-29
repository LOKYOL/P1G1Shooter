#include <stdio.h>
#include <stdlib.h>
#include "projectile.h"

void InitProj(projectile** proj, int speed, int direction, int PosX, int PosY, int SizeX, int SizeY, char Color)
{
	(*proj) = (projectile*)malloc(sizeof(projectile));
	(*proj)->projEntity->vitesse = speed;
	(*proj)->direction = direction;
	(*proj)->PosX = PosX;
	(*proj)->SizeX = SizeX;
	(*proj)->SizeY = SizeY;
	(*proj)->Color = Color;
}

void RefreshProjMovement(projectile** proj, int deltatime)
{
	if ((*proj)->direction)
	{
		(*proj)->PosX += (*proj)->projEntity->vitesse * deltatime;
	}
	else
	{
		(*proj)->PosX += -(*proj)->projEntity->vitesse * deltatime;
	}
}
