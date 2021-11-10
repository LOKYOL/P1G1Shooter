// P1G1Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Engine/Game.h"

#include "PlayerStruct.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "EnemyShooter.h"
#include "Enemy.h"
#include <conio.h>
#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	printf(
		"Entity : %d\n"
		"Player : %d\n"
		"Obstacle : %d\n"
		"Projectile : %d\n"
		"EnemyShooter : %d\n"
		"Enemy K : %d\n"
		"DisplayZone : %d",
		sizeof(Entity), sizeof(Player), sizeof(Obstacle), 
		sizeof(Projectile), sizeof(EnemyShooter), sizeof(Enemy),
		sizeof(DisplayZone)
	);
	_getch();

	Game	game;
	InitGame(&game);

	int tempclock = 0, ms = 17;

	while (MainLoop(&game) == 0)
	{
		//while(clock() < tempclock + ms){} // <- limite d'fps
		//tempclock = clock();
	}

	CloseGame(&game);

	return 0;
}
