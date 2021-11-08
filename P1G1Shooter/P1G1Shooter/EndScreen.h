#pragma once
#include "GameState.h"
#include "ConsoleDisplay.h"

typedef struct EndScreenData
{
	DisplayZone mCredit;

}EndScreenData;

int EndScreenInit(struct Game* game, struct GameState* state);
int EndScreenClose(struct Game* game, struct GameState* state);
int EndScreenUpdate(struct Game* game, struct GameState* state);