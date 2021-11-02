#include "Game.h"
#include "TitleScreen.h"


void	InitGame(Game* game)
{
	game->mDisplaySettings = InitDisplay(180, 60, 8, 0);

	InitTime();

	game->mGameTime = GetTime();
	game->mGameDt = 0.0;


	game->mStateStack = DVectorCreate();
	DVectorInit(game->mStateStack, sizeof(GameState), 0, NULL);


	GameState	title;
	title.mStateInit = &TitleScreenInit;
	title.mStateClose = &TitleScreenClose;
	title.mStateUpdate = &TitleScreenUpdate;

	PushGameState(game, title);

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

	int updateResult=0;

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
void	ChangeGameState(Game* game, GameState state)
{
	PopGameState(game);
	PushGameState(game, state);
}