#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Engine/Game.h"

#include <crtdbg.h>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(x);

	srand(time(0));

	Game	game;
	InitGame(&game);

	int tempclock = ZERO, ms = 17;

	while (MainLoop(&game) == 0)
	{
		//while(clock() < tempclock + ms){} // <- limite d'fps
		//tempclock = clock();
	}

	CloseGame(&game);

	return ZERO;
}
