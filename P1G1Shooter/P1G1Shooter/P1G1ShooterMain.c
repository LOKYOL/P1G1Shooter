// P1G1Shooter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Engine/Game.h"

int main()
{
	Game	game;
	InitGame(&game);

	int tempclock = 0, ms = 50; //50ms per frame = 20fps

	while (MainLoop(&game) == 0)
	{
		while(clock() < tempclock + ms){}
		tempclock = clock();
	}

	CloseGame(&game);

	return 0;
}
