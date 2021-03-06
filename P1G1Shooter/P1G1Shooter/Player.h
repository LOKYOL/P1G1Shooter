#pragma once

#include "Entity.h"
#include "GameScreen.h"

#define PLAYER_INIT_SECTION "Player"

#define HIT_TIME			0.5f // Invincibility frame

#define PlayerUpdate(player, inputs, deltaTime) \
	Player_Update((void*)player, &inputs, deltaTime)

typedef struct Player
{
	Entity mEntity;

	DisplayZone*	mChargeZone;
	DisplayZone*	mHealthZone;

	double			mTouchedTime;

	double			mCurrentReloadCooldown; // Refactoring : Struct Weapon ?
	double			mMaxReloadCooldown;

	double			mReloadGain;

	double			mOverheatCooldown;
	double			mMaxOverheatCooldown;

	double			mShootAimAssistTimer;

	float			mMaxEnergy;
	float			mCurrentEnergy;

	float			mShootCost;

	
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

/// <summary>
/// Execute this function when player collide with something
/// </summary>
/// <param name="current">Current player</param>
/// <param name="entity">Entity which collide with Player</param>
/// <param name="game">Current game</param>
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
void Player_Shoot(Player* player, GameScreenData* data, Game* game);

void Player_Shoot_Standard(Player* player, GameScreenData* data, Game* game);

void Player_Shoot_AimAssist(Player* player, GameScreenData* data, Game* game);

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

void Player_TakeDamages(Player* player, Game* game);

/// <summary>
/// Free components of a player and destroy him
/// </summary>
/// <param name="entity">Entity casted to player to destroy</param>
void Player_Destroy(Player* player);

/// <summary>
/// Check collision of a player projectile
/// </summary>
/// <param name="current">Current player projectile</param>
/// <param name="entity">Other entity which collide with the current </param>
/// <param name="game">Current game</param>
void PlayerProjectile_OnCollide(struct Projectile* current, Entity* entity, Game* game);
