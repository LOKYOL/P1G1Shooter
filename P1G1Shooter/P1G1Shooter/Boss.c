#include "Boss.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Player.h"
#include "Projectile.h"
#include <math.h>

void Boss_Initialize(Boss** _boss, GameScreenData* _gameScreen)
{
 	Boss* newBoss = (Boss*)malloc(sizeof(Boss));
	memset(newBoss, 0, sizeof(Boss));

	*_boss = newBoss;

	Entity_Initialize(newBoss, TYPE_ENEMY_BOSS, 
		WINDOW_WIDTH, (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY) / 2, 
		BOSS_HEALTH, BOSS_SPEED,
		&_gameScreen->mSprites[TYPE_ENEMY_BOSS],
		Boss_Update, Boss_OnCollide, Boss_Destroy);

	newBoss->mShootCooldown = 2;
	newBoss->mChangeDirectionCooldown = 1;
	newBoss->mCurrentDirectionX = (rand() % 3) - 1;
	newBoss->mCurrentDirectionY = (rand() % 3) - 1;
	newBoss->mHitTimer = 0;

	newBoss->mCurrentPhaseUpdate = Boss_PhaseA_Update;
	newBoss->mCurrentMovementUpdate = Boss_Movement_EnterScreen;
}

void Boss_Update(Boss* boss, Game* _game, GameScreenData* _gameScreen)
{
	boss->mCurrentPhaseUpdate(boss, _game, _gameScreen);

	if (boss)
	{
		if (boss->mHitTimer > 0)
		{
			boss->mHitTimer -= _game->mGameDt;

			// SPRITE HITTED
			if(boss->mHitTimer <= 0)
			{
				if (boss->mCurrentPhaseUpdate == Boss_PhaseA_Update)
				{
					boss->mEntity.mDisplayZone.mBuffer = _gameScreen->mSprites[TYPE_ENEMY_BOSS].mBuffer;
				}
				else
				{
					boss->mEntity.mDisplayZone.mBuffer = _gameScreen->mSprites[TYPE_ENEMY_BOSS + 2].mBuffer;
				}
			}
			else
			{
				boss->mEntity.mDisplayZone.mBuffer = _gameScreen->mSprites[TYPE_ENEMY_BOSS + 1].mBuffer;
			}
		}

		boss->mCurrentMovementUpdate(boss, _game, _gameScreen);
		if (boss->mShootCooldown > 0)
		{
			boss->mShootCooldown -= _game->mGameDt;
		}
		else
		{
			Enemy_Shoot(boss, _gameScreen);
		}

		FlushDisplayZone(_game->mDisplaySettings, &boss->mEntity.mDisplayZone);
	}
}

void Boss_PhaseA_Update(Boss* boss, Game* _game, GameScreenData* _data)
{
	if (boss->mShootCooldown > 0)
	{
		boss->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Boss_Shoot(boss, _data);
	}

	if (boss->mEntity.mHealth <= 0)
	{
		boss->mCurrentPhaseUpdate = Boss_PhaseB_Update;
		boss->mEntity.mHealth = 30;
	}
}

void Boss_PhaseB_Update(Boss* boss, Game* game, GameScreenData* data)
{
	
}

void Boss_Movement_EnterScreen(Boss* boss, Game* game, GameScreenData* data)
{
	if (boss->mEntity.mPosition_x > WINDOW_WIDTH - boss->mEntity.mDisplayZone.mSizeX - 5)
	{
		boss->mCurrentDirectionX = -1;
	}
	else
	{
		boss->mCurrentDirectionX = 0;
	}

	if (boss->mEntity.mPosition_y > 0)
	{
		boss->mCurrentDirectionY = -1;
	}
	else
	{
		boss->mCurrentDirectionY = 0;
	}

	if (boss->mCurrentDirectionX != 0 || boss->mCurrentDirectionY != 0)
	{
		Entity_Move
		(
			boss, 
			boss->mCurrentDirectionX * BOSS_SPEED * game->mGameDt, 
			boss->mCurrentDirectionY * BOSS_SPEED * game->mGameDt
		);
	}
	else
	{
		boss->mCurrentMovementUpdate = Boss_Movement_UpDown;
		boss->mCurrentDirectionX = 0;
		boss->mCurrentDirectionY = -1;
	}
}

void Boss_Movement_UpDown(Boss* _boss, GameScreenData* _gameScreen, Game* _game)
{
	if (_boss->mEntity.mPosition_y >= WINDOW_HEIGHT - _boss->mEntity.mDisplayZone.mSizeY)
	{
		_boss->mCurrentDirectionY = -BOSS_SPEED * _game->mGameDt;
	}
	else if (_boss->mEntity.mPosition_y <= 0)
	{
		_boss->mCurrentDirectionY = BOSS_SPEED * _game->mGameDt;
	}

	// Apply movement
	Entity_Move(&_boss->mEntity, _boss->mCurrentDirectionX, _boss->mCurrentDirectionY);
}

void Boss_Shoot(Boss* boss, GameScreenData* _gameScreen)
{
	Projectile* newProjectile;
	Proj_Initialize(&newProjectile, 10, 1,
		-1, 0, 
		boss->mEntity.mPosition_x + EYE_LEFT_POS_X, 
		boss->mEntity.mPosition_y + EYE_LEFT_POS_Y,
		TYPE_ENEMY_PROJECTILE, _gameScreen,
		Projectile_Update, Projectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);

	boss->mShootCooldown = 1;
}

void Boss_OnCollide(Boss* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER_PROJECTILE:
		Entity_TakeDamages(_current, 1);
		_current->mHitTimer = 0.5;
		if (_current->mEntity.mHealth > 0)
		{
			Play_Sound("enemy_hit", game->mSoundManager);
		}
		else
		{
			Play_Sound("enemy_die", game->mSoundManager);
		}
		return;
	default:
		return;
	}
}

void Boss_Destroy(Entity* _entity)
{
	free(_entity);
}
