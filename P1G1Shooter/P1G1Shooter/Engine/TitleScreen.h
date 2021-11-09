#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"

#define PushTitleScreen(game) GameState	title; \
	title.mStateInit = &TitleScreenInit; \
	title.mStateClose = &TitleScreenClose; \
	title.mStateUpdate = &TitleScreenUpdate; \
	PushGameState(game, title);

typedef struct TitleScreenData
{
	DisplayZone	mZoneTitre;

	DisplayZone mOptionsZone;

	DisplayZone mKeybindsZone;

	DisplayZone mTitleAsciiZone;
	char** mOptions;
	int mNbOptions;

	int mCurrentSelector;

	int mTempClock;
	int mCurrentColor;
}TitleScreenData;

/// <summary>
/// Initialize a title screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int TitleScreenInit(struct Game* game, struct GameState* state);

/// <summary>
/// Closes a title screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int TitleScreenClose(struct Game* game, struct GameState* state);

/// <summary>
/// Updates a title screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int TitleScreenUpdate(struct Game* game, struct GameState* state);

/// <summary>
/// Prints every menu options
/// </summary>
/// <param name="datascreen">Current screen data</param>
void PrintOptions(TitleScreenData* datascreen);

/// <summary>
/// Prints a menu option
/// </summary>
/// <param name="datascreen">Current screen data</param>
/// <param name="index">Current selected menu option</param>
void PrintOption(TitleScreenData* datascreen, int index);

/// <summary>
/// Pushes the game screen
/// </summary>
/// <param name="game">Game to push</param>
void PushGamescreen(struct Game* game);

/// <summary>
/// Sets the color of ascii art title
/// <summary>
void SetTitleColor(DisplayZone* dz, int FColor, int BColor);
