#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"

typedef struct TitleScreenData
{
	DisplayZone	mZoneTitre;


}TitleScreenData;

int TitleScreenInit(struct Game* game, struct GameState* state);
int TitleScreenClose(struct Game* game, struct GameState* state);
int TitleScreenUpdate(struct Game* game, struct GameState* state);

