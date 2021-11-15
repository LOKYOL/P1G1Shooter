#pragma once

#include "Entity.h"
#include "GameScreen.h"

#define PLAYER_SPEED		WINDOW_HEIGHT / 3

#define MAX_ENERGY			100.f
#define RELOAD_SPEED		25.f
#define SHOOT_COST			10.f
#define RELOAD_COOLDOWN		0.5f
#define OVERHEAT_COOLDOWN	5.f

#define PlayerUpdate(player, inputs, deltaTime) \
	Player_Update((void*)player, &inputs, deltaTime)

typedef struct Player
{
	Entity mEntity;

	DisplayZone*	mChargeZone;
	DisplayZone*	mHealthZone;

	float mCurrentEnergy;
	float mReloadCooldown;
	float mShootCooldown;
} Player;

/// <summary>
/// Initialize a player
/// </summary>
/// <param name="player">Player to initialize</param>
/// <param name="gameScreen">Gamescreen to display player on</param>
void InitPlayer(Player** player, GameScreenData* gameScreen);

/// <summary>
/// Update the player's
/// </summary>
/// <param name="player">Player the update</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Player_Update(void* player, struct Game* game, GameScreenData* gameScreen);

/// <summary>
/// Update the player's movements
/// </summary>
/// <param name="player">Player the update</param>
/// <param name="game">Current game</param>
void Player_UpdateMovement(Player* player, struct Game* game);

void Player_OnCollide(Player* current, Entity* entity, Game* game);

/// <summary>
/// Prevents a player from going out of bound 
/// </summary>
/// <param name="player">Player to check</param>
/// <param name="posX">X position to check</param>
/// <param name="posY">Y position to check</param>
void ClampPlayerPos(Player* player, double* posX, double* posY);

/// <summary>
/// Makes the player shoot
/// </summary>
/// <param name="player">Player to update</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="gameStruct">Current game</param>
void Player_Shoot(Player* player, GameScreenData* gameScreen, Game* gameStruct);

/// <summary>
/// Updates the overshoot display of the player
/// </summary>
/// <param name="player">Player to check</param>
void UpdateBatteryDisplayZonePosition(Player* player);

/// <summary>
/// Updates the health bar display of the player
/// </summary>
/// <param name="player">Player to check</param>
void UpdateHealthDisplayZonePosition(Player* player);

/// <summary>
/// Creates the overshoot display of the player
/// </summary>
/// <param name="player">Player to check</param>
void DrawBatteryInDisplayZone(Player* player);

/// <summary>
/// Creates the health bar display of the player
/// </summary>
/// <param name="player">Player to check</param>
void DrawHealthInDisplayZone(Player* player);

/// <summary>
/// Free components of a player and destroy him
/// </summary>
/// <param name="entity">Entity casted to player to destroy</param>
void Player_Destroy(Player* player);

/// <summary>
/// Free components of a player projectiles and destroy him
/// </summary>
/// <param name="entity">Entity casted to player projectile to destroy</param>
void PlayerProjectile_OnCollide(struct Projectile* current, Entity* entity, Game* game);
