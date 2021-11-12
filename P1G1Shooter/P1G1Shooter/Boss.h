#pragma once

#include "Engine/Game.h"
#include "GameScreen.h"

typedef struct Boss
{
	Entity mEntity;

	double mShootCooldown;
	double mChangeDirectionCooldown;

	int mCurrentDirectionX;
	int mCurrentDirectionY;
}Boss;

/// <summary>
/// Initialize a boss with given parameters
/// </summary>
/// <param name="boss">Boss to initialize</param>
/// <param name="health"> Health of boss</param>
/// <param name="damage">Damage dealt by boss</param>
/// <param name="speed">Speed of boss</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Initialize(Boss** boss,
	unsigned int health, int damage, int speed,
	GameScreenData* gameScreen);

/// <summary>
/// Update an enemy shooter
/// </summary>
/// <param name="boss">Boss to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Update(void* boss, Game* game, GameScreenData* gameScreen);

/// <summary>
/// Change the position of a boss
/// </summary>
/// <param name="boss">Boss to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void Boss_UpdateMovement(Boss* boss, GameScreenData* gameScreen, Game* game);

/// <summary>
/// Makes the boss shoot
/// </summary>
/// <param name="boss">Boss to update</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Shoot(Boss* boss, GameScreenData* gameScreen);

void Boss_OnCollide(Entity* entity);

void Boss_Destroy(Entity* entity);
