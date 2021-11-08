#include "TitleScreen.h"
#include "Game.h"
#include "../GameScreen.h"

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
		"FISH KILLER", 0, NO_FLAG);

	InitDisplayZone(&datascreen->mOptionsZone, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	datascreen->mNbOptions = 2;
	datascreen->mOptions = malloc(sizeof(char*) * datascreen->mNbOptions);
	datascreen->mOptions[0] = "New Game";
	datascreen->mOptions[1] = "Exit";

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

	FlushDisplayZone(game->mDisplaySettings, &datascreen->mZoneTitre);
	FlushDisplayZone(game->mDisplaySettings, &datascreen->mOptionsZone);

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