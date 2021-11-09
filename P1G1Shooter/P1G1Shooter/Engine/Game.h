#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"
#include "../Inputs.h"
#include "../Entity.h"

#define WINDOW_HEIGHT		120
#define WINDOW_WIDTH		220
#define WINDOW_FONT_SIZE	8
#define WINDOW_FONT_SQUARED	1

#define OBSTACLE_SPAWN_TIMER	0.6
#define ENEMY_SPAWN_TIMER	1.5

typedef struct Game
{
	DisplaySettings*	mDisplaySettings;

	Inputs*				mInputs;

	DVector*			mStateStack;

	double				mGameTime;
	double				mGameDt;

	int					mScore;
	DisplayZone			mScoreDisplayZone;
}Game;

/// <summary>
/// Initialize the game
/// </summary>
/// <param name="game">Game to initialize</param>
void	InitGame(Game* game);

/// <summary>
/// Closes the game and free all alocated memory
/// </summary>
/// <param name="game">Game to close</param>
void	CloseGame(Game* game);

/// <summary>
/// Continues the main game loop
/// </summary>
/// <param name="game">Game to run</param>
/// <returns>0 to keep going, anything else if finished/error</returns>
int		MainLoop(Game* game);

/// <summary>
/// Pushes a gamestate
/// </summary>
/// <param name="game">Game to update</param>
/// <param name="state">Gamestate to push</param>
void	PushGameState(Game* game, GameState state);

/// <summary>
/// Pops a gamestate
/// </summary>
/// <param name="game">Game to get gamestate from</param>
void	PopGameState(Game* game);

/// <summary>
/// Swiches the current gamestate with another
/// </summary>
/// <param name="game">Game to switch gamestate from</param>
/// <param name="state">New gamestate</param>
void	ChangeGameState(Game* game, GameState state);

