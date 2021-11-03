#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"
#include "../Inputs.h"
#include "../Entity.h"

#define WINDOW_HEIGHT		50
#define WINDOW_WIDTH		100
#define WINDOW_FONT_SIZE	13
#define WINDOW_FONT_SQUARED	1

#define OBSTACLE_SPAWN_TIMER	5

typedef struct Game
{
	DisplaySettings*	mDisplaySettings;

	Inputs*				mInputs;

	DVector*			mStateStack;

	DVector*			mAllEntities;


	double				mGameTime;
	double				mGameDt;
	double				mGameSpawnObstacleTimer;
}Game;

void	InitGame(Game* game);
void	CloseGame(Game* game);
int		MainLoop(Game* game);

void	PushGameState(Game* game, GameState state);
void	PopGameState(Game* game);
void	ChangeGameState(Game* game, GameState state);

void	SpawnObstacle(Game* game);

void	PushEntity(Game* game, Entity** entity);
void	PopEntity(Game* game, Entity* entity);

DVector GetAllEntityOfType(Game* game, EntityType type);

char	CompareCollision(Entity* entityA, Entity* entityB);

void PopBackIfIsDead(Game* game, Entity* entity);
