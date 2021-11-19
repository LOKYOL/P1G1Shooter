#include "Game.h"
#include "TimeManagement.h"
#include "TitleScreen.h"
#include "../GameScreen.h"
#include "../Player.h"
#include "../Obstacle.h"
#include "../Projectile.h"
#include "../EndScreen.h"

void	InitGame(Game* _game)
{
	_game->mScore = ZERO;

	_game->mDisplaySettings = InitDisplay
	(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FONT_SIZE,
		WINDOW_FONT_SQUARED
	);

	InitTime();

	_game->mGameTime = GetTime();
	_game->mGameDt = 0.0;

	_game->mStateStack = DVectorCreate();
	DVectorInit(_game->mStateStack, sizeof(GameState), ZERO, NULL);

	InitInputs(&_game->mInputs);

	_game->mSoundManager = (SoundManager*)malloc(sizeof(SoundManager));
	_game->mSoundManager->mSound = NULL;

	InitSoundManager(_game->mSoundManager);

	_game->mDifficulty = 0;

	PushTitleScreen(_game);
}

void	CloseGame(Game* _game)
{
	// clear stack
	while (DVectorSize(_game->mStateStack))
	{
		PopGameState(_game);
	}

	DVectorDestroy(_game->mStateStack);

	DestroyInputs(_game->mInputs);

	free(_game->mSoundManager);

	CloseDisplay(_game->mDisplaySettings);
}

int		MainLoop(Game* _game)
{
	double currentTime = GetTime();
	_game->mGameDt = currentTime - _game->mGameTime;
	_game->mGameTime = currentTime;

	UpdateAllInputs(_game->mInputs);

	int updateResult = ZERO;

	if (DVectorSize(_game->mStateStack))
	{
		GameState current = (DVectorBackTyped(_game->mStateStack, GameState));
		updateResult = current.mStateUpdate(_game, &current);
	}

	
	SwapBuffer(_game->mDisplaySettings);

	return updateResult;
}

void	PushGameState(Game* _game, GameState _state)
{
	DVectorPushBack(_game->mStateStack, &_state);

	GameState* currentState = DVectorBack(_game->mStateStack);
	currentState->mStateInit(_game, currentState);

}

void	PopGameState(Game* _game)
{
	if (DVectorSize(_game->mStateStack))
	{
		GameState* currentState = DVectorBack(_game->mStateStack);
		currentState->mStateClose(_game, currentState);

		DVectorPopBack(_game->mStateStack);
	}
}

void ChangeGameState(Game* _game, GameState _state)
{
	PopGameState(_game);
	PushGameState(_game, _state);
}
