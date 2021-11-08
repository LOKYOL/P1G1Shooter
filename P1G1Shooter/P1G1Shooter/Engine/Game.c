#include "Game.h"
#include "TimeManagement.h"
#include "TitleScreen.h"
#include "../GameScreen.h"
#include "../PlayerStruct.h"
#include "../Obstacle.h"
#include "../Projectile.h"
#include "../EndScreen.h"

void	InitGame(Game* game)
{
	game->mScore = 0;
	InitDisplayZone(&game->mScoreDisplayZone, 0, 0, 10, 10, 0);
	PrintInDisplayZone(&game->mScoreDisplayZone, WHITE, BLACK, 0, 0, "Score: ", 0, NO_FLAG);

	game->mDisplaySettings = InitDisplay
	(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FONT_SIZE,
		WINDOW_FONT_SQUARED
	);

	InitTime();

	game->mGameTime = GetTime();
	game->mGameDt = 0.0;

	game->mStateStack = DVectorCreate();
	DVectorInit(game->mStateStack, sizeof(GameState), 0, NULL);

	InitInputs(&game->mInputs);

	PushTitleScreen(game);
	//GameState	title;
	title.mStateInit = &TitleScreenInit;
	title.mStateClose = &TitleScreenClose;
	title.mStateUpdate = &TitleScreenUpdate;

	PushGameState(game, title);

	GameState	gameScreen;
	gameScreen.mStateInit = &GameScreenInit;
	gameScreen.mStateClose = &GameScreenClose;
	gameScreen.mStateUpdate = &GameScreenUpdate;

	PushGameState(game, gameScreen);
}

void	CloseGame(Game* game)
{
	// clear stack
	while (DVectorSize(game->mStateStack))
	{
		PopGameState(game);
	}

	DVectorDestroy(game->mStateStack);

	CloseDisplay(game->mDisplaySettings);
}

int		MainLoop(Game* game)
{
	double currentTime = GetTime();
	game->mGameDt = currentTime - game->mGameTime;
	game->mGameTime = currentTime;

	ClearBuffer(game->mDisplaySettings, WHITE, DARKER | BLUE);

	UpdateAllInputs(game->mInputs);

	int updateResult = 0;

	if (DVectorSize(game->mStateStack))
	{
		GameState current = (DVectorBackTyped(game->mStateStack, GameState));
		updateResult = current.mStateUpdate(game, &current);
	}

	
	SwapBuffer(game->mDisplaySettings);

	return updateResult;
}

void	PushGameState(Game* game, GameState state)
{
	DVectorPushBack(game->mStateStack, &state);

	GameState* currentState = DVectorBack(game->mStateStack);
	currentState->mStateInit(game, currentState);

}

void	PopGameState(Game* game)
{
	if (DVectorSize(game->mStateStack))
	{
		GameState* currentState = DVectorBack(game->mStateStack);
		currentState->mStateClose(game, currentState);

		DVectorPopBack(game->mStateStack);
	}
}

void ChangeGameState(Game* _game, GameState _state)
{
	PopGameState(_game);
	PushGameState(_game, _state);
}
