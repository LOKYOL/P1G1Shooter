#include "EndScreen.h"
#include "Game.h"
#include "Engine/DisplayZoneDrawing.h"


int EndScreenInit(struct Game* game, struct GameState* state)
{
	state->mData = malloc(sizeof(EndScreenData));

	EndScreenData* datascreen = (EndScreenData*)state->mData;
	datascreen->mCredit = *(CreateDisplayZoneFromBMP("pascontent.bmp"));
	
	PrintInDisplayZone(
		&datascreen->mCredit, 
		MAGENTA, BLACK, 
		WINDOW_WIDTH / 2 - 10, WINDOW_HEIGHT / 2 - 10, 
		"GAME OVER", 0, NO_FLAG);

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

