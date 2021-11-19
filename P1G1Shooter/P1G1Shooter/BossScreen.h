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

/// <summary>
/// Initialize the boss screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
int BossScreenInit(struct Game* game, struct GameState* gameState);

/// <summary>
/// Update the boss screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
int BossScreenUpdate(struct Game* game, struct GameState* gameState);

/// <summary>
/// Closes the boss screen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
int BossScreenClose(struct Game* game, struct GameState* gameState);

/// <summary>
/// Ends the boss phase and resumes normal gameplay
/// </summary>
/// <param name="boss">Boss to edit</param>
/// <param name="game">Current game</param>
void EndBoss(struct Boss* boss, struct Game* game);
