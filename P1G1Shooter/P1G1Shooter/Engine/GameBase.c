
#include "Game.h"

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