#include "TitleScreen.h"
#include "Game.h"
#include "../GameScreen.h"
#include "DisplayZoneDrawing.h"
#include <time.h>

int TitleScreenInit(struct Game* game, struct GameState* state)
{
	state->mData = malloc(sizeof(TitleScreenData));
	memset(state->mData, 0, sizeof(TitleScreenData));

	TitleScreenData* datascreen = (TitleScreenData*)state->mData;

	InitDisplayZone(&datascreen->mZoneTitre, WINDOW_WIDTH / 2 - 6, WINDOW_HEIGHT / 3, 15, 1, 0);
	PrintInDisplayZone(
		&datascreen->mZoneTitre,
		BRIGHT_BLUE, BLACK,
		0, 0,
		"BUBBLE WARS", 0, NO_FLAG);

	InitDisplayZone(&datascreen->mOptionsZone, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	datascreen->mNbOptions = 2;
	datascreen->mOptions = malloc(sizeof(char*) * datascreen->mNbOptions);
	datascreen->mOptions[0] = "New Game";
	datascreen->mOptions[1] = "Exit";

	datascreen->mTempClock = 0;
	datascreen->mCurrentColor = 1;

	InitDisplayZone(&datascreen->mKeybindsZone, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	
	datascreen->mKeybindsZone = *CreateDisplayZoneFromBMP("keybinds.bmp");
	datascreen->mKeybindsZone.mPosX = WINDOW_WIDTH / 2 - 40;
	datascreen->mKeybindsZone.mPosY = WINDOW_HEIGHT - 40;

	InitDisplayZone(&datascreen->mTitleAsciiZone, 0, 0, 100, 6, 0);
	int asciiPosX = 0, asciiPosY = 0;
	SetTitleColor(&datascreen->mTitleAsciiZone, 6, 0);
	datascreen->mTitleAsciiZone.mPosX = 74;
	datascreen->mTitleAsciiZone.mPosY = 32;
	return 0;
}
int TitleScreenClose(struct Game* game, struct GameState* state)
{
	TitleScreenData* datascreen = (TitleScreenData*)state->mData;
	CloseDisplayZone(&datascreen->mZoneTitre);

	/*for (int i = 0; i < datascreen->mNbOptions; i++)
	{
		free((datascreen->mOptions[i]));
	}*/
	free(datascreen->mOptions);

	free(state->mData);
	ClearBuffer(game->mDisplaySettings, BLACK, BLACK);

	return 0;
}

int TitleScreenUpdate(struct Game* game, struct GameState* state)
{
	TitleScreenData* datascreen = (TitleScreenData*)state->mData;

	ClearBuffer(game->mDisplaySettings, WHITE, BLACK);

	if (KeyPressStart(*game->mInputs, VK_RETURN))
	{
		switch (datascreen->mCurrentSelector)
		{
		case 0:
			PushGamescreen(game);
			break;
		case 1:
			return 1;
			break;
		default:
			break;
		}
	}
	else
	{
		int selector = datascreen->mCurrentSelector;
		if (KeyPressStart(*game->mInputs, VK_DOWN))
		{
			selector++;
			selector %= datascreen->mNbOptions;
		}
		if (KeyPressStart(*game->mInputs, VK_UP))
		{
			selector--;
			selector = (selector + datascreen->mNbOptions) % datascreen->mNbOptions;
		}
		datascreen->mCurrentSelector = selector;

		PrintOptions(datascreen);
	}

	FlushDisplayZone(game->mDisplaySettings, &datascreen->mOptionsZone);
	FlushDisplayZone(game->mDisplaySettings, &datascreen->mZoneTitre);
	FlushDisplayZone(game->mDisplaySettings, &datascreen->mKeybindsZone);

	if (datascreen->mTempClock < clock() - 400) {
		datascreen->mCurrentColor++;
		if (datascreen->mCurrentColor >= 15) { datascreen->mCurrentColor = 1; }
		datascreen->mTempClock = clock();
		SetTitleColor(&datascreen->mTitleAsciiZone, datascreen->mCurrentColor, 0);
	}

	FlushDisplayZone(game->mDisplaySettings, &datascreen->mTitleAsciiZone);

	return 0;
}

void PrintOptions(TitleScreenData* _datascreen)
{
	for (int i = 0; i < _datascreen->mNbOptions; i++)
	{
		PrintOption(_datascreen, i);
	}
}

void PrintOption(TitleScreenData* _datascreen, int _index)
{
	ConsoleColors fg = BRIGHT_BLUE;
	if (_index == _datascreen->mCurrentSelector && fg == BRIGHT_BLUE)
	{
		fg = RED;
	}
	else
	{
		fg = BRIGHT_BLUE;
	}

	PrintInDisplayZone(
		&_datascreen->mOptionsZone,
		fg, BLACK,
		WINDOW_WIDTH / 2 - strlen(_datascreen->mOptions[_index]) / 2, 
		WINDOW_HEIGHT / 3 + 3 + _index * 2,
		_datascreen->mOptions[_index], 0, NO_FLAG
	);
}

void PushGamescreen(struct Game* game)
{
	PushGameScreen(game);
}

void SetTitleColor(DisplayZone* dz, int FColor, int BColor) {
	PrintInDisplayZone(dz, FColor, BColor, 0, 0, "\260\333\333\333\333\333\273\260\333\333\333\273\260\260\333\333\273\260\333\333\333\333\333\333\273\260\333\333\333\333\333\333\273\260\333\333\273\260\260\260\333\333\273\040\040\333\333\333\333\333\333\333\273\333\333\273\260\333\333\333\333\333\333\273\333\333\273\260\260\333\333\273", 0, NO_FLAG);
	PrintInDisplayZone(dz, FColor, BColor, 0, 1, "\333\333\311\315\315\333\333\273\333\333\333\333\273\260\333\333\272\333\333\311\315\315\315\315\274\260\333\333\311\315\315\333\333\273\310\333\333\273\260\333\333\311\274\040\040\333\333\311\315\315\315\315\274\333\333\272\333\333\311\315\315\315\315\274\333\333\272\260\260\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(dz, FColor, BColor, 0, 2, "\333\333\333\333\333\333\333\272\333\333\311\333\333\273\333\333\272\333\333\272\260\260\333\333\273\260\333\333\333\333\333\333\311\274\260\310\333\333\333\333\311\274\260\040\040\333\333\333\333\333\273\260\260\333\333\272\310\333\333\333\333\333\273\260\333\333\333\333\333\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(dz, FColor, BColor, 0, 3, "\333\333\311\315\315\333\333\272\333\333\272\310\333\333\333\333\272\333\333\272\260\260\310\333\333\273\333\333\311\315\315\333\333\273\260\260\310\333\333\311\274\260\260\040\040\333\333\311\315\315\274\260\260\333\333\272\260\310\315\315\315\333\333\273\333\333\311\315\315\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(dz, FColor, BColor, 0, 4, "\333\333\272\260\260\333\333\272\333\333\272\260\310\333\333\333\272\310\333\333\333\333\333\333\311\274\333\333\272\260\260\333\333\272\260\260\260\333\333\272\260\260\260\040\040\333\333\272\260\260\260\260\260\333\333\272\333\333\333\333\333\333\311\274\333\333\272\260\260\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(dz, FColor, BColor, 0, 5, "\310\315\274\260\260\310\315\274\310\315\274\260\260\310\315\315\274\260\310\315\315\315\315\315\274\260\310\315\274\260\260\310\315\274\260\260\260\310\315\274\260\260\260\040\040\310\315\274\260\260\260\260\260\310\315\274\310\315\315\315\315\315\274\260\310\315\274\260\260\310\315\274", 0, NO_FLAG);
}