#include "EndScreen.h"
#include "Game.h"
#include "Engine/DisplayZoneDrawing.h"


int EndScreenInit(struct Game* game, struct GameState* state)
{
	char num[10];

	_itoa_s(game->mScore, num, 10, 10);

	char totalScore[18] = "Score: ";

	for (int i = 7; i < 18; i++) {
		totalScore[i] = num[i - 7];
	}


	game->mScoreDisplayZone.mPosX = WINDOW_WIDTH/2-4;
	game->mScoreDisplayZone.mPosY = WINDOW_HEIGHT/2;
	PrintInDisplayZone(&game->mScoreDisplayZone, MAGENTA, BLACK, 0, 0, totalScore, 0, NO_FLAG);

	state->mData = malloc(sizeof(EndScreenData));

	EndScreenData* datascreen = (EndScreenData*)state->mData;
	datascreen->mCredit = *(CreateDisplayZoneFromBMP("gameover.bmp"));
	
	PrintInDisplayZone(
		&datascreen->mCredit, 
		MAGENTA, BLACK, 
		WINDOW_WIDTH / 2-4, WINDOW_HEIGHT / 2 - 2, 
		"GAME OVER", 0, NO_FLAG);

	PrintInDisplayZone(
		&datascreen->mCredit,
		WHITE, BLACK,
		180, 110,
		"Press return to continue...", 0, NO_FLAG);

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
	FlushDisplayZone(game->mDisplaySettings, &game->mScoreDisplayZone);
	if (KeyPressStart(*game->mInputs, VK_RETURN))
	{
		PopGameState(game);
		PopGameState(game);
	}

	return 0;
}

