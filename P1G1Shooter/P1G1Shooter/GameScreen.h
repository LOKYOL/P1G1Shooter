#pragma once

#include "Engine/GameState.h"
#include "Engine/ConsoleDisplay.h"
#include "Engine/DVector.h"
#include "Entity.h"
#include "EndScreen.h"
#include "Engine/Game.h"

#define BOSS_SCORE	10

#define PushGameScreen(game)	GameState gameScreen; \
	gameScreen.mStateInit = &GameScreenInit; \
	gameScreen.mStateClose = &GameScreenClose; \
	gameScreen.mStateUpdate = &GameScreenUpdate; \
	PushGameState(game, gameScreen);

typedef struct GameScreenData
{
	DVector*			mAllEntities;

	struct Player*		mPlayer;

	DisplayZone*		mSprites;

	DisplayZone*		mScoreDisplayZone;

	double				mGameSpawnObstacleTimer;
	double				mGameSpawnEnemyTimer;
	double				mGameSpawnEnemyKamikazeTimer;
}GameScreenData;

/// <summary>
/// Initialize the gamescreen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if successful</returns>
int GameScreenInit(struct Game* game, struct GameState* state);

/// <summary>
/// Closes the game, reseting all memory allocated from the game
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if successful</returns>
int GameScreenClose(struct Game* game, struct GameState* state);

/// <summary>
/// Updates all elements in the gamescreen
/// </summary>
/// <param name="game">Current game</param>
/// <param name="state">Current gamestate</param>
/// <returns>0 if successful</returns>
int GameScreenUpdate(struct Game* game, struct GameState* state);

/// <summary>
/// Push an entity in the vector of all already existing entities
/// </summary>
/// <param name="game">Current game</param>
/// <param name="entity">Entity to add</param>
void	PushEntity(GameScreenData* game, Entity** entity);

/// <summary>
/// Push an entity in the vector of all already existing entities
/// </summary>
/// <param name="game">Current game</param>
/// <param name="entity">Entity to add</param>
void	PopEntity(GameScreenData* game, Entity* entity);

/// <summary>
/// Check collisions between all entities from vector
/// </summary>
/// <param name="list">Vector of all entities</param>
/// <param name="gameStruct">Current gamescreen</param>
void HandleCollision(DVector* list, Game* gameStruct);

/// <summary>
/// Check if an entity collide with any other, and applies the correct action if there is a collision
/// </summary>
/// <param name="entity">Entity to check</param>
/// <param name="list">List of other entities</param>
/// <param name="length">Size of entity list</param>
/// <param name="gameStruct">Current gamescreen</param>
void HandleEntityCollision(Entity* entity, Entity** list, int length, Game* game);

void HandleEntitiesCollision(Entity* entityA, Entity* entityB, Game* game);

/// <summary>
/// Check if 2 entities are colliding
/// </summary>
/// <param name="entityA">First entity</param>
/// <param name="entityB">Second entity</param>
/// <returns>1 if colliding, 0 if not</returns>
char	CompareCollision(Entity* entityA, Entity* entityB);

/// <summary>
/// Check if a value is inbetween 2 others
/// </summary>
/// <param name="value">Value to check</param>
/// <param name="min">Minimum value</param>
/// <param name="max">Maximum value</param>
/// <returns>1 if inbetween, 0 if not</returns>
char	InRange(int value, int min, int max);

/// <summary>
/// Remove an entity if its health points reach 0
/// </summary>
/// <param name="game">Current gamescreen</param>
/// <param name="entity">Entity to check</param>
char	PopBackIfIsDead(GameScreenData* game, Entity* entity, Game* gameStruct);

/// <summary>
/// Spawn entities randomly
/// </summary>
/// <param name="game">Current game</param>
/// <param name="data">Current gamescreen</param>
void	SpawnEntity(struct Game* game, GameScreenData* data);

/// <summary>
/// Spawn an obstacle
/// </summary>
/// <param name="game">Current gamescreen</param>
void	SpawnObstacle(GameScreenData* game);

/// <summary>
/// Spawn an enemy
/// </summary>
/// <param name="game">Current gamescreen</param>
void	SpawnEnemy(GameScreenData* game);

/// <summary>
/// Spawn a kamikaze
/// </summary>
/// <param name="game">Current gamescreen</param>
void	SpawnEnemyKamikaze(GameScreenData* game);

/// <summary>
/// Updates all entities from a game
/// </summary>
/// <param name="game">Current game</param>
/// <param name="game">Current gamescreen</param>
void	UpdateEntity(struct Game* game, GameScreenData* data);

/// <summary>
/// Update the player's weapon stats
/// </summary>
/// <param name="game">Current game</param>
/// <param name="game">Current gamescreen</param>
void	UpdateWeapon(struct Game* game, GameScreenData* data);

/// <summary>
/// ENds the game if the player is dead
/// </summary>
/// <param name="game">Current game</param>
/// <param name="player">Current player</param>
void	EndGame(struct Game* game, struct Player* player);
