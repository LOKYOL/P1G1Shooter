#pragma once

typedef struct projectile {
	int speed;
	int direction; //0->360
	int PosX, PosY;
	int SizeX, SizeY;
	char Color;
}projectile;

void InitProj();

void lzshf();