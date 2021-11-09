#pragma once

#include "Entity.h"
#include "GameScreen.h"

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

	DisplayZone	mChargeZone;
	DisplayZone	mHealthZone;

	float mCurrentEnergy;
	float mReloadCooldown;
	float mShootCooldown;
} Player;

void InitPlayer(Player** player);

void Player_Update(void* player, struct Game* game, GameScreenData* gameScreen);

void Player_UpdateMovement(Player* player, struct Game* game);

void ClampPlayerPos(Player* player, double* posX, double* posY);

void Player_Shoot(Player* player, GameScreenData* _gameScreen);

void UpdateBatteryDisplayZonePosition(Player* player);

void UpdateHealthDisplayZonePosition(Player* player);

void DrawBatteryInDisplayZone(Player* player);

void DrawHealthInDisplayZone(Player* player);
