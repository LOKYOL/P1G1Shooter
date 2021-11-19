#include "EndScreen.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"
#include <stdio.h>


int EndScreenInit(struct Game* _game, struct GameState* _state)
{
	_state->mData = malloc(sizeof(EndScreenData));

	EndScreenData* datascreen = (EndScreenData*)_state->mData;
	
	datascreen->mCredit = CreateDisplayZoneFromBMP("Sprites/gameover.bmp");
	
	PrintInDisplayZone(
		datascreen->mCredit, 
		MAGENTA, BLACK, 
		WINDOW_WIDTH / 2 - 4, WINDOW_HEIGHT / 2 - 2, 
		"GAME OVER", 0, NO_FLAG);

	PrintInDisplayZone(
		datascreen->mCredit,
		WHITE, BLACK,
		WINDOW_WIDTH - 40, WINDOW_HEIGHT - 10,
		"Press return to continue...", 0, NO_FLAG);

	datascreen->mScoreDisplayZone = malloc(sizeof(DisplayZone));
	InitDisplayZone(datascreen->mScoreDisplayZone, 
		WINDOW_WIDTH / 2 - 4, WINDOW_HEIGHT / 2, 25, 1, 0);

	char totalScore[20] = "";

	snprintf(totalScore, 19, "Score : %d", _game->mScore);

	PrintInDisplayZone(datascreen->mScoreDisplayZone, MAGENTA, BLACK, 0, 0, totalScore, 0, NO_FLAG);

	return 0;

}
int EndScreenClose(struct Game* _game, struct GameState* _state)
{
	EndScreenData* datascreen = (EndScreenData*)_state->mData;

	CloseDisplayZone(datascreen->mCredit);
	free(datascreen->mCredit);
	CloseDisplayZone(datascreen->mScoreDisplayZone);
	free(datascreen->mScoreDisplayZone);

	free(_state->mData);
	ClearBuffer(_game->mDisplaySettings, BLACK, BLACK, ' ');

	return 0;
}
int EndScreenUpdate(struct Game* _game, struct GameState* _state)
{
	EndScreenData* datascreen = (EndScreenData*)_state->mData;
	FlushDisplayZone(_game->mDisplaySettings, datascreen->mCredit);
	FlushDisplayZone(_game->mDisplaySettings, datascreen->mScoreDisplayZone);
	if (KeyPressStart(*_game->mInputs, VK_RETURN))
	{
		_game->mScore = 0;
		while (_game->mStateStack->mCurrentSize > 1)
		{
			PopGameState(_game);
		}
	}

	return 0;
}

