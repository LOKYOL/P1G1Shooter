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
	DisplayZone* mCredit;

	DisplayZone* mScoreDisplayZone;

}EndScreenData;

/// <summary>
/// Initialize an end screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int EndScreenInit(struct Game* game, struct GameState* state);

/// <summary>
/// Close an end screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int EndScreenClose(struct Game* game, struct GameState* state);

/// <summary>
/// Update an end screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if no error</returns>
int EndScreenUpdate(struct Game* game, struct GameState* state);
