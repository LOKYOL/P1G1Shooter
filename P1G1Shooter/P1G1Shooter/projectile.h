#pragma once
#include "Entity.h"

typedef struct projectile {
	Entity* projEntity;
	unsigned char direction; //0 = left, 1 = right
	int PosX;
	int SizeX, SizeY;
	char Color;
}projectile;

void InitProj(projectile** proj, int speed, int direction, int PosX, int PosY, int SizeX, int SizeY, char Color);

void RefreshProjMovement(projectile** proj, int deltatime);