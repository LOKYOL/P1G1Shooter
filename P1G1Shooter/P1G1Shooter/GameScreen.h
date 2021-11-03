#pragma once

#include "GameState.h"
#include "ConsoleDisplay.h"
#include "DVector.h"
#include "Entity.h"

typedef struct GameScreenData
{
	DVector*			mAllEntities;

	double				mGameSpawnObstacleTimer;
}GameScreenData;

int GameScreenInit(struct Game* game, struct GameState* state);
int GameScreenClose(struct Game* game, struct GameState* state);
int GameScreenUpdate(struct Game* game, struct GameState* state);


void	PushEntity(GameScreenData* game, Entity** entity);
void	PopEntity(GameScreenData* game, Entity* entity);

DVector* GetAllEntityOfType(GameScreenData* game, EntityType type);

char	CompareCollision(Entity* entityA, Entity* entityB);

void PopBackIfIsDead(GameScreenData* game, Entity* entity);

void	SpawnObstacle(GameScreenData* game);
