#pragma once

#include "Engine/Game.h"
#include "BossScreen.h"

#define BOSS_HEALTH		60
#define BOSS_PHASE_B_HEALTH	40
#define BOSS_SPEED		20

#define EYE_LEFT_POS_X	10
#define EYE_LEFT_POS_Y	6

#define EYE_RIGHT_POS_X	28
#define EYE_RIGHT_POS_Y	8

#define MOUNTH_POS_X	15
#define MOUNTH_POS_Y	25

#define BOSS_SHOOT_COOLDOWN 0.3
#define SPAWNKAMIKAZE_TIMER	1

typedef void (*BossPhaseUpdate)(struct Boss*, Game*, struct GameScreenData*);

typedef struct Boss
{
	Entity	mEntity;

	double	mShootCooldown;
	double	mChangeDirectionCooldown;

	double	mHitTimer;

	double	mCurrentDirectionX;
	double	mCurrentDirectionY;

	double	mSpawnKamikazeCooldown;
	char	mCanSpawnKamikaze;

	BossPhaseUpdate mCurrentPhaseUpdate;
	BossPhaseUpdate mCurrentMovementUpdate;
}Boss;

/// <summary>
/// Initialize a boss with given parameters
/// </summary>
/// <param name="boss">Boss to initialize</param>
/// <param name="health"> Health of boss</param>
/// <param name="damage">Damage dealt by boss</param>
/// <param name="speed">Speed of boss</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Initialize(Boss** boss, GameScreenData* gameScreen);

/// <summary>
/// Update an enemy shooter
/// </summary>
/// <param name="boss">Boss to edit</param>
/// <param name="game">Current game</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Update(Boss* boss, Game* game, struct GameScreenData* gameScreen);

void Boss_PhaseA_Update(Boss* boss, Game* game, struct GameScreenData* data);

void Boss_PhaseB_Update(Boss* boss, Game* game, struct GameScreenData* data);

// MOVEMENTS PHASES

void Boss_Movement_EnterScreen(Boss* boss, Game* game, struct GameScreenData* data);
void Boss_Movement_Re_EnterScreen(Boss* boss, Game* game, struct GameScreenData* data);

/// <summary>
/// Change the position of a boss
/// </summary>
/// <param name="boss">Boss to edit</param>
/// <param name="gameScreen">Datas bind to the game state</param>
/// <param name="game">Current game</param>
void Boss_Movement_UpDown(Boss* boss, Game* game, struct GameScreenData* gameScreen);

void Boss_Movement_Dash(Boss* boss, Game* game, struct GameScreenData* gameScreen);

/// <summary>
/// Makes the boss shoot
/// </summary>
/// <param name="boss">Boss to update</param>
/// <param name="gameScreen">Datas bind to the game state</param>
void Boss_Shoot(Boss* boss, struct GameScreenData* gameScreen);

void Boss_SpawnKamikaze(Boss* boss, Game* game, struct GameScreenData* gameScreen);
void Boss_SpawnShooter(Boss* boss, Game* game, struct GameScreenData* gameScreen);

void Boss_TakeDamages(Boss* boss, float damages);

void Boss_OnCollide(Boss* current, Entity* entity, Game* game);

void Boss_Destroy(Entity* entity);

