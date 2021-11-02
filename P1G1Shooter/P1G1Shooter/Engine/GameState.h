#pragma once

typedef int (*gameStateInit)(struct Game* game, struct GameState* state);
typedef int (*gameStateClose)(struct Game* game, struct GameState* state);
typedef int (*gameStateUpdate)(struct Game* game, struct GameState* state);

typedef struct GameState
{
	gameStateInit	mStateInit;
	gameStateClose	mStateClose;
	gameStateUpdate	mStateUpdate;

	void*			mData;

}GameState;