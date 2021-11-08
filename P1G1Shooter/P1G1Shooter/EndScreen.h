#pragma once
#include "Engine/GameState.h"
#include "Engine/ConsoleDisplay.h"

#define PushEndScreen(game) GameState endScreen; \
	endScreen.mStateInit = &EndScreenInit; \
	endScreen.mStateClose = &EndScreenClose; \
	endScreen.mStateUpdate = &EndScreenUpdate; \
	PushGameState(game, endScreen);

typedef struct EndScreenData
{
	DisplayZone mCredit;

}EndScreenData;

int EndScreenInit(struct Game* game, struct GameState* state);
int EndScreenClose(struct Game* game, struct GameState* state);
int EndScreenUpdate(struct Game* game, struct GameState* state);