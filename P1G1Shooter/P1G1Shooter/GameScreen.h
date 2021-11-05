#pragma once

#include "Engine/GameState.h"
#include "Engine/ConsoleDisplay.h"
#include "Engine/DVector.h"
#include "Entity.h"


typedef struct GameScreenData
{
	DVector*			mAllEntities;

	struct Player*				mPlayer;

	double				mGameSpawnObstacleTimer;
	double				mGameSpawnEnemyTimer;
}GameScreenData;

int GameScreenInit(struct Game* game, struct GameState* state);
int GameScreenClose(struct Game* game, struct GameState* state);
int GameScreenUpdate(struct Game* game, struct GameState* state);


void	PushEntity(GameScreenData* game, Entity** entity);
void	PopEntity(GameScreenData* game, Entity* entity);

DVector* GetAllEntityOfType(GameScreenData* game, EntityType type);

void HandleCollision(DVector* list);
void HandleEntityCollision(Entity* entity, Entity** list, int length);

char	CompareCollision(Entity* entityA, Entity* entityB);
char	InRange(int value, int min, int max);
char	CanCollide(Entity* entityA, Entity* entityB);

void	PopBackIfIsDead(GameScreenData* game, Entity* entity);

void	SpawnObstacle(GameScreenData* game);

void	SpawnEnemy(GameScreenData* game);