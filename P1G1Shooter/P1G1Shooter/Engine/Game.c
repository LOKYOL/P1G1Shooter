#include "Game.h"
#include "TitleScreen.h"
#include "../Entity.h"
#include "../PlayerStruct.h"


void	InitGame(Game* game)
{
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

	game->mAllEntities = DVectorCreate();
	DVectorInit(game->mAllEntities, sizeof(Entity*), 0, NULL);

	InitInputs(&game->mInputs);

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer);
	Entity* myEntity = &myPlayer->entity;
	PushEntity(game, &myEntity);


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
	ClearBuffer(game->mDisplaySettings, WHITE, BLACK);

	UpdateAllInputs(game->mInputs);

	double currentTime = GetTime();
	game->mGameDt = currentTime - game->mGameTime;
	game->mGameTime = currentTime;

	int updateResult=0;

	if (DVectorSize(game->mStateStack))
	{
		GameState current = (DVectorBackTyped(game->mStateStack, GameState));
		updateResult = current.mStateUpdate(game, &current);
	}

	Entity* curEntity = NULL;

	// FOR EACH ENTITY
	for (int i = 0; i < game->mAllEntities->mCurrentSize; i++)
	{
		curEntity = *(Entity**)DVectorGet(game->mAllEntities, i);

		if (curEntity->update != NULL)
		{
			curEntity->update(curEntity, game);
		}
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

void PushEntity(Game* _game, Entity** _entity)
{
	DVectorPushBack(_game->mAllEntities, _entity);
}

void PopEntity(Game* _game, Entity* _entity)
{
	for (int i = 0; i < _game->mAllEntities->mCurrentSize; i++)
	{
		if (DVectorGet(_game->mAllEntities, i) == _entity)
		{
			DVectorErase(_game->mAllEntities, i);
		}
	}
}
