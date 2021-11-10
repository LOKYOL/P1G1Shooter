#include "EndScreen.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"
#include <stdio.h>


int EndScreenInit(struct Game* game, struct GameState* state)
{
	char num[10];

	_itoa_s(game->mScore, num, 10, 10);

	char totalScore[18] = "Score: ";

	snprintf(totalScore, 17, "Score : %d", num);

	game->mScoreDisplayZone->mPosX = WINDOW_WIDTH/2-4;
	game->mScoreDisplayZone->mPosY = WINDOW_HEIGHT/2;
	PrintInDisplayZone(game->mScoreDisplayZone, MAGENTA, BLACK, 0, 0, totalScore, 0, NO_FLAG);

	state->mData = malloc(sizeof(EndScreenData));

	EndScreenData* datascreen = (EndScreenData*)state->mData;
	
	datascreen->mCredit = CreateDisplayZoneFromBMP("gameover.bmp");
	
	PrintInDisplayZone(
		datascreen->mCredit, 
		MAGENTA, BLACK, 
		WINDOW_WIDTH / 2-4, WINDOW_HEIGHT / 2 - 2, 
		"GAME OVER", 0, NO_FLAG);

	PrintInDisplayZone(
		datascreen->mCredit,
		WHITE, BLACK,
		180, 110,
		"Press return to continue...", 0, NO_FLAG);

	return 0;

}
int EndScreenClose(struct Game* game, struct GameState* state)
{
	EndScreenData* datascreen = (EndScreenData*)state->mData;
	CloseDisplayZone(datascreen->mCredit);

	free(state->mData);

	ClearBuffer(game->mDisplaySettings, BLACK, BLACK);

	return 0;
}
int EndScreenUpdate(struct Game* game, struct GameState* state)
{
	EndScreenData* datascreen = (EndScreenData*)state->mData;
	FlushDisplayZone(game->mDisplaySettings, datascreen->mCredit);
	FlushDisplayZone(game->mDisplaySettings, game->mScoreDisplayZone);
	if (KeyPressStart(*game->mInputs, VK_RETURN))
	{
		game->mScore = 0;
		game->mScoreDisplayZone->mPosX = 0;
		game->mScoreDisplayZone->mPosY = 0;
		PrintInDisplayZone(game->mScoreDisplayZone, WHITE, BLACK, 0, 0, "Score:      ", 0, NO_FLAG);
		PopGameState(game);
		PopGameState(game);
	}

	return 0;
}

