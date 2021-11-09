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
	char** mOptions;
	int mNbOptions;

	int mCurrentSelector;
}TitleScreenData;

int TitleScreenInit(struct Game* game, struct GameState* state);
int TitleScreenClose(struct Game* game, struct GameState* state);
int TitleScreenUpdate(struct Game* game, struct GameState* state);

void PrintOptions(TitleScreenData* datascreen);
void PrintOption(TitleScreenData* datascreen, int index);

void PushGamescreen(struct Game* game);