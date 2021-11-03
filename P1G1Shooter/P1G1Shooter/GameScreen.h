#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"
#include "DVector.h"

typedef struct GameScreenData
{
	DisplayZone	mZoneTitre;

	DVector* mAllEntities;

	double				mGameSpawnObstacleTimer;
}GameScreenData;

int GameScreenInit(struct Game* game, struct GameState* state);
int GameScreenClose(struct Game* game, struct GameState* state);
int GameScreenUpdate(struct Game* game, struct GameState* state);