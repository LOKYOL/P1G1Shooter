#pragma once

#include "Engine/GameState.h"
#include "Engine/ConsoleDisplay.h"
#include "Engine/DVector.h"
#include "Entity.h"
#include "EndScreen.h"
#include "Engine/Game.h"

#define PushGameScreen(game)	GameState gameScreen; \
	gameScreen.mStateInit = &GameScreenInit; \
	gameScreen.mStateClose = &GameScreenClose; \
	gameScreen.mStateUpdate = &GameScreenUpdate; \
	PushGameState(game, gameScreen);

typedef struct GameScreenData
{
	DVector*			mAllEntities;

	struct Player*				mPlayer;

	double				mGameSpawnObstacleTimer;
	double				mGameSpawnEnemyTimer;
	double				mGameSpawnEnemyKamikazeTimer;
}GameScreenData;

int GameScreenInit(struct Game* game, struct GameState* state);
int GameScreenClose(struct Game* game, struct GameState* state);
int GameScreenUpdate(struct Game* game, struct GameState* state);


void	PushEntity(GameScreenData* game, Entity** entity);
void	PopEntity(GameScreenData* game, Entity* entity);

DVector* GetAllEntityOfType(GameScreenData* game, EntityType type);

void HandleCollision(DVector* list, Game* gameStruct);
void HandleEntityCollision(Entity* entity, Entity** list, int length, Game* gameStruct);

char	CompareCollision(Entity* entityA, Entity* entityB);
char	InRange(int value, int min, int max);
char	CanCollide(Entity* entityA, Entity* entityB);

void	PopBackIfIsDead(GameScreenData* game, Entity* entity);

void	SpawnEntity(struct Game* game, GameScreenData* data);

void	SpawnObstacle(GameScreenData* game);

void	SpawnEnemy(GameScreenData* game);

void	SpawnEnemyKamikaze(GameScreenData* game);

void	UpdateEntity(struct Game* game, GameScreenData* data);

void	UpdateWeapon(struct Game* game, GameScreenData* data);

void	EndGame(struct Game* game, struct Player* player);
