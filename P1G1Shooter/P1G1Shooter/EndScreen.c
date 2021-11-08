#include "EndScreen.h"
#include "Game.h"


int EndScreenInit(struct Game* game, struct GameState* state)
{
	state->mData = malloc(sizeof(EndScreenData));

	EndScreenData* datascreen = (EndScreenData*)state->mData;

	InitDisplayZone(&datascreen->mCredit, 30, 20, 100, 10, 0);
	PrintInDisplayZone(&datascreen->mCredit, RED, BLUE, 5, 3, "C est la fin du jeu", 0, NO_FLAG);

	return 0;

}
int EndScreenClose(struct Game* game, struct GameState* state)
{
	EndScreenData* datascreen = (EndScreenData*)state->mData;
	CloseDisplayZone(&datascreen->mCredit);

	free(state->mData);

	ClearBuffer(game->mDisplaySettings, BLACK, BLACK);

	return 0;
}
int EndScreenUpdate(struct Game* game, struct GameState* state)
{
	EndScreenData* datascreen = (EndScreenData*)state->mData;
	FlushDisplayZone(game->mDisplaySettings, &datascreen->mCredit);
	if (KeyPressStart(*game->mInputs, VK_RETURN))
	{
		PopGameState(game);
		PopGameState(game);
	}

	return 0;
}

