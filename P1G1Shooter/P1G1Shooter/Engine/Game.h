#pragma once
#include "ConsoleDisplay.h"
#include "TimeManagement.h"
#include "DVector.h"
#include "GameState.h"


typedef struct Game
{
	DisplaySettings*	mDisplaySettings;

	DVector*			mStateStack;

	double				mGameTime;
	double				mGameDt;
}Game;



void	InitGame(Game* game);
void	CloseGame(Game* game);
int		MainLoop(Game* game);

void	PushGameState(Game* game, GameState state);
void	PopGameState(Game* game);
void	ChangeGameState(Game* game, GameState state);

