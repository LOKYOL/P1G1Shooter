#pragma once

typedef struct projectile {
	int speed;
	int direction; //0->360
	int PosX, PosY;
	int SizeX, SizeY;
	char Color;
}projectile;

void InitProj(projectile** proj, int speed, int direction, int PosX, int PosY, int SizeX, int SizeY, char Color);

void lzshf();