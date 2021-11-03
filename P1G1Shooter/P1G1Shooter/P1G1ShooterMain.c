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



	while (MainLoop(&game) == 0)
	{
		Sleep(1);
	}

	CloseGame(&game);

	return 0;
}
