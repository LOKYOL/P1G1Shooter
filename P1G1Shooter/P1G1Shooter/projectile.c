#include <stdio.h>
#include <stdlib.h>
#include "projectile.h"

void InitProj(projectile** proj, int speed, int direction, int PosX, int PosY, int SizeX, int SizeY, char Color) {
	(*proj) = (projectile*)malloc(sizeof(projectile));
	(*proj)->speed = speed;
	(*proj)->direction = direction;
	(*proj)->PosX = PosX;
	(*proj)->PosY = PosY;
	(*proj)->SizeX = SizeX;
	(*proj)->SizeY = SizeY;
	(*proj)->Color = Color;
}
