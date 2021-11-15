#pragma once

#include "GameScreen.h"

#define PushBossScreen(game)	GameState gameScreen; \
	gameScreen.mStateInit = &BossScreenInit; \
	gameScreen.mStateClose = &BossScreenClose; \
	gameScreen.mStateUpdate = &BossScreenUpdate; \
	PushGameState(game, gameScreen);

typedef void (*BossPhaseUpdate)(Game*, struct BossScreenData*);

typedef struct BossScreenData
{
	GameScreenData* mGameScreenData;

	struct Boss* mBoss;

} BossScreenData;

int BossScreenInit(struct Game* game, struct GameState* gameState);

int BossScreenUpdate(struct Game* game, struct GameState* gameState);

int BossScreenClose(struct Game* game, struct GameState* gameState);

void EndBoss(struct Boss* boss, struct Game* game);
