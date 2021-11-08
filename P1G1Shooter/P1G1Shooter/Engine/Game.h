#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"
#include "../Inputs.h"
#include "../Entity.h"

#define WINDOW_HEIGHT		120
#define WINDOW_WIDTH		220
#define WINDOW_FONT_SIZE	8
#define WINDOW_FONT_SQUARED	1

#define OBSTACLE_SPAWN_TIMER	0.4
#define ENEMY_SPAWN_TIMER	1.5

typedef struct Game
{
	DisplaySettings*	mDisplaySettings;

	Inputs*				mInputs;

	DVector*			mStateStack;

	double				mGameTime;
	double				mGameDt;

	int mScore;
}Game;

void	InitGame(Game* game);
void	CloseGame(Game* game);
int		MainLoop(Game* game);

void	PushGameState(Game* game, GameState state);
void	PopGameState(Game* game);
void	ChangeGameState(Game* game, GameState state);

