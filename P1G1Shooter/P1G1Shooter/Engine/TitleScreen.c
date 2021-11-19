#include "TitleScreen.h"
#include "Game.h"
#include "../GameScreen.h"
#include "DisplayZoneDrawing.h"
#include <time.h>

int TitleScreenInit(struct Game* _game, struct GameState* _state)
{
	_state->mData = malloc(sizeof(TitleScreenData));
	memset(_state->mData, 0, sizeof(TitleScreenData));

	TitleScreenData* datascreen = (TitleScreenData*)_state->mData;
	datascreen->mOptionsZone = (DisplayZone*)malloc(sizeof(DisplayZone));
	datascreen->mTitleAsciiZone = (DisplayZone*)malloc(sizeof(DisplayZone));

	InitDisplayZone(datascreen->mOptionsZone, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	datascreen->mNbOptions = 2;
	datascreen->mOptions = malloc(sizeof(char*) * datascreen->mNbOptions);
	datascreen->mOptions[0] = "New Game";
	datascreen->mOptions[1] = "Exit";

	datascreen->mTempClock = 0;
	datascreen->mCurrentColor = 1;

	//InitDisplayZone(datascreen->mKeybindsZone, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	
	datascreen->mKeybindsZone = CreateDisplayZoneFromBMP("Sprites/keybinds.bmp");
	datascreen->mKeybindsZone->mPosX = WINDOW_WIDTH / 2 - 40;
	datascreen->mKeybindsZone->mPosY = WINDOW_HEIGHT - 40;

	InitDisplayZone(datascreen->mTitleAsciiZone, 0, 0, 100, 6, 0);
	int asciiPosX = 0, asciiPosY = 0;
	SetTitleColor(datascreen->mTitleAsciiZone, 6, 0);
	datascreen->mTitleAsciiZone->mPosX = 74;
	datascreen->mTitleAsciiZone->mPosY = 32;
	return 0;
}

int TitleScreenClose(struct Game* _game, struct GameState* _state)
{
	TitleScreenData* datascreen = (TitleScreenData*)_state->mData;
	CloseDisplayZone(datascreen->mOptionsZone);
	CloseDisplayZone(datascreen->mKeybindsZone);
	CloseDisplayZone(datascreen->mTitleAsciiZone);

	free(datascreen->mOptions);

	free(datascreen->mOptionsZone);
	free(datascreen->mKeybindsZone);
	free(datascreen->mTitleAsciiZone);

	free(_state->mData);
	ClearBuffer(_game->mDisplaySettings, BLACK, BLACK, ' ');

	return 0;
}

int TitleScreenUpdate(struct Game* _game, struct GameState* _state)
{
	TitleScreenData* data = (TitleScreenData*)_state->mData;

	ClearBuffer(_game->mDisplaySettings, WHITE, BLACK, ' ');

	if (HandleKeyPress(_game, data))
		return 1;

	FlushDisplayZone(_game->mDisplaySettings, data->mOptionsZone);
	FlushDisplayZone(_game->mDisplaySettings, data->mKeybindsZone);

	if (data->mTempClock < clock() - 400) {
		data->mCurrentColor++;
		if (data->mCurrentColor >= 15) { data->mCurrentColor = 1; }
		data->mTempClock = clock();
		SetTitleColor(data->mTitleAsciiZone, data->mCurrentColor, 0);
	}

	FlushDisplayZone(_game->mDisplaySettings, data->mTitleAsciiZone);

	return 0;
}

char HandleKeyPress(Game* _game, TitleScreenData* _data)
{
	if (KeyPressStart(*_game->mInputs, VK_RETURN) || 
		KeyPressStart(*_game->mInputs, VK_SPACE))
	{
		EnterKeyPressed(_game, _data);

		return _data->mCurrentSelector;
	}
	else
	{
		if (KeyPressStart(*_game->mInputs, VK_DOWN) || 
			KeyPressStart(*_game->mInputs, 'S'))
		{
			SelectorDown(_game, _data);
		}

		if (KeyPressStart(*_game->mInputs, VK_UP) || KeyPressStart(*_game->mInputs, 'Z'))
		{
			SelectorUp(_game, _data);
		}

		PrintOptions(_data);
	}

	FlushDisplayZone(_game->mDisplaySettings, _data->mOptionsZone);
	FlushDisplayZone(_game->mDisplaySettings, _data->mKeybindsZone);
	
	return 0;
}

void EnterKeyPressed(struct Game* _game, TitleScreenData* _data)
{
	switch (_data->mCurrentSelector)
	{
	case 0:
		PushGamescreen(_game);
		Play_Sound("new_game.wav", _game->mSoundManager);
		break;
	default: break;
	}
}

void SelectorDown(struct Game* _game, TitleScreenData* _data)
{
	int selector = _data->mCurrentSelector;

	selector++;
	selector %= _data->mNbOptions;

	_data->mCurrentSelector = selector;

	Play_Sound("click.wav", _game->mSoundManager);
}

void SelectorUp(struct Game* _game, TitleScreenData* _data)
{
	int selector = _data->mCurrentSelector;

	selector--;
	selector = (selector + _data->mNbOptions) % _data->mNbOptions;

	_data->mCurrentSelector = selector;

	Play_Sound("click.wav", _game->mSoundManager);
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
		_datascreen->mOptionsZone,
		fg, BLACK,
		WINDOW_WIDTH / 2 - strlen(_datascreen->mOptions[_index]) / 2, 
		WINDOW_HEIGHT / 3 + 3 + _index * 2,
		_datascreen->mOptions[_index], 0, NO_FLAG
	);
}

void PushGamescreen(struct Game* _game)
{
	PushGameScreen(_game);
}

void SetTitleColor(DisplayZone* _dz, int _FColor, int _BColor) 
{
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 0, "\260\333\333\333\333\333\273\260\333\333\333\273\260\260\333\333\273\260\333\333\333\333\333\333\273\260\333\333\333\333\333\333\273\260\333\333\273\260\260\260\333\333\273\040\040\333\333\333\333\333\333\333\273\333\333\273\260\333\333\333\333\333\333\273\333\333\273\260\260\333\333\273", 0, NO_FLAG);
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 1, "\333\333\311\315\315\333\333\273\333\333\333\333\273\260\333\333\272\333\333\311\315\315\315\315\274\260\333\333\311\315\315\333\333\273\310\333\333\273\260\333\333\311\274\040\040\333\333\311\315\315\315\315\274\333\333\272\333\333\311\315\315\315\315\274\333\333\272\260\260\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 2, "\333\333\333\333\333\333\333\272\333\333\311\333\333\273\333\333\272\333\333\272\260\260\333\333\273\260\333\333\333\333\333\333\311\274\260\310\333\333\333\333\311\274\260\040\040\333\333\333\333\333\273\260\260\333\333\272\310\333\333\333\333\333\273\260\333\333\333\333\333\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 3, "\333\333\311\315\315\333\333\272\333\333\272\310\333\333\333\333\272\333\333\272\260\260\310\333\333\273\333\333\311\315\315\333\333\273\260\260\310\333\333\311\274\260\260\040\040\333\333\311\315\315\274\260\260\333\333\272\260\310\315\315\315\333\333\273\333\333\311\315\315\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 4, "\333\333\272\260\260\333\333\272\333\333\272\260\310\333\333\333\272\310\333\333\333\333\333\333\311\274\333\333\272\260\260\333\333\272\260\260\260\333\333\272\260\260\260\040\040\333\333\272\260\260\260\260\260\333\333\272\333\333\333\333\333\333\311\274\333\333\272\260\260\333\333\272", 0, NO_FLAG);
	PrintInDisplayZone(_dz, _FColor, _BColor, 0, 5, "\310\315\274\260\260\310\315\274\310\315\274\260\260\310\315\315\274\260\310\315\315\315\315\315\274\260\310\315\274\260\260\310\315\274\260\260\260\310\315\274\260\260\260\040\040\310\315\274\260\260\260\260\260\310\315\274\310\315\315\315\315\315\274\260\310\315\274\260\260\310\315\274", 0, NO_FLAG);
}