#include "Boss.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Player.h"
#include "Projectile.h"
#include "EnemyKamikaze.h"
#include "EnemyShooter.h"
#include <math.h>

void Boss_Initialize(Boss** _boss, GameScreenData* _gameScreen)
{
 	Boss* newBoss = (Boss*)malloc(sizeof(Boss));
	memset(newBoss, 0, sizeof(Boss));

	*_boss = newBoss;

	Entity_Initialize((Entity*)newBoss, TYPE_ENEMY_BOSS, 
		WINDOW_WIDTH, (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY) / 2, 
		BOSS_HEALTH, BOSS_SPEED,
		&_gameScreen->mSprites[TYPE_ENEMY_BOSS],
		Boss_Update, Boss_OnCollide, Boss_Destroy);

	newBoss->mCurrentPhaseUpdate = Boss_PhaseA_Update;
	newBoss->mCurrentMovementUpdate = Boss_Movement_EnterScreen;

	newBoss->mShootCooldown = 1;
	newBoss->mChangeDirectionCooldown = 1;
	newBoss->mCurrentDirectionX = RandomInt(-1, 1);
	newBoss->mCurrentDirectionY = RandomInt(-1, 1);
	newBoss->mHitTimer = 0;

	newBoss->mSpawnKamikazeCooldown = SPAWNKAMIKAZE_TIMER;
	newBoss->mCanSpawnKamikaze = 0;
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
					boss->mEntity.mDisplayZone.mBuffer = 
						_gameScreen->mSprites[TYPE_ENEMY_BOSS].mBuffer;
				}
				else
				{
					boss->mEntity.mDisplayZone.mBuffer = 
						_gameScreen->mSprites[TYPE_ENEMY_BOSS + 2].mBuffer;
				}
			}
			else
			{
				boss->mEntity.mDisplayZone.mBuffer = 
					_gameScreen->mSprites[TYPE_ENEMY_BOSS + 1].mBuffer;
			}
		}

		boss->mCurrentMovementUpdate(boss, _game, _gameScreen);
		if (boss->mShootCooldown > 0)
		{
			boss->mShootCooldown -= _game->mGameDt;
		}
		else
		{
			Enemy_Shoot((Entity*)boss, _gameScreen);
		}

		if (boss->mCanSpawnKamikaze && boss->mSpawnKamikazeCooldown <= 0)
		{
			_gameScreen->mGameSpawnEnemyKamikazeTimer -= ENEMY_KAMIKAZE_SPAWN_TIMER;

			if (rand() % 2)
			{
				Boss_SpawnKamikaze(boss, _game, _gameScreen);
			}
			else
			{
				Boss_SpawnShooter(boss, _game, _gameScreen);
			}
			boss->mSpawnKamikazeCooldown = SPAWNKAMIKAZE_TIMER;
		}
		else
		{
			boss->mSpawnKamikazeCooldown -= _game->mGameDt;
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

	if (boss->mEntity.mHealth <= BOSS_PHASE_B_HEALTH)
	{
		boss->mCanSpawnKamikaze = 0;
		boss->mCurrentPhaseUpdate = Boss_PhaseB_Update;
		boss->mCurrentMovementUpdate = Boss_Movement_Dash;
	}
}

void Boss_PhaseB_Update(Boss* _boss, Game* _game, GameScreenData* _data)
{
	if (_boss->mShootCooldown > 0)
	{
		_boss->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Boss_Shoot(_boss, _data);
	}
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
			(Entity*)boss, 
			boss->mCurrentDirectionX * boss->mEntity.mSpeed * game->mGameDt, 
			boss->mCurrentDirectionY * boss->mEntity.mSpeed * game->mGameDt
		);
	}
	else
	{
		boss->mCurrentMovementUpdate = Boss_Movement_UpDown;
		boss->mCurrentDirectionX = 0;
		boss->mCurrentDirectionY = -1;
	}
}

void Boss_Movement_Re_EnterScreen(Boss* boss, Game* game, GameScreenData* data)
{
	if (boss->mEntity.mPosition_x > WINDOW_WIDTH - boss->mEntity.mDisplayZone.mSizeX - 5)
	{
		Entity_Move((Entity*)boss, -boss->mEntity.mSpeed * game->mGameDt, 0);
	}
	else
	{
		boss->mCurrentMovementUpdate = Boss_Movement_UpDown;
		boss->mCurrentDirectionX = 0;
		boss->mCurrentDirectionY = -1;
	}
}

void Boss_Movement_UpDown(Boss* _boss, Game* _game, struct GameScreenData* _gameScreen)
{
	if (_boss->mEntity.mPosition_y >= WINDOW_HEIGHT - _boss->mEntity.mDisplayZone.mSizeY)
	{
		_boss->mCurrentDirectionY = -_boss->mEntity.mSpeed * _game->mGameDt;
	}
	else if (_boss->mEntity.mPosition_y <= 0)
	{
		_boss->mCurrentDirectionY = _boss->mEntity.mSpeed * _game->mGameDt;
	}

	// Apply movement
	Entity_Move(&_boss->mEntity, _boss->mCurrentDirectionX, _boss->mCurrentDirectionY);
}

void Boss_Movement_Dash(Boss* _boss, Game* _game, struct GameScreenData* _gameScreen)
{
	if (_boss->mEntity.mPosition_x < -(int)_boss->mEntity.mDisplayZone.mSizeX)
	{
		Entity_MoveTo((Entity*)_boss, WINDOW_WIDTH, _boss->mEntity.mPosition_y);
		_boss->mEntity.mSpeed = BOSS_SPEED;
		_boss->mCanSpawnKamikaze = 1;
		_boss->mCurrentMovementUpdate = Boss_Movement_Re_EnterScreen;
	}
	else
	{
		_boss->mEntity.mSpeed += (float)_game->mGameDt * BOSS_SPEED;
		Entity_Move((Entity*)_boss, -_boss->mEntity.mSpeed * _game->mGameDt, 0);
	}
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

void Boss_SpawnKamikaze(Boss* _boss, Game* _game, GameScreenData* _gameScreen)
{
	EnemyKamikaze* newEnemy = NULL;
	Enemy_Initialize(&newEnemy, _gameScreen);
	Entity_MoveTo((Entity*)newEnemy, 
		_boss->mEntity.mPosition_x + MOUNTH_POS_X, 
		_boss->mEntity.mPosition_y + MOUNTH_POS_Y);
	DVectorPushBack(_gameScreen->mAllEntities, &newEnemy);
}

void Boss_SpawnShooter(Boss* _boss, Game* _game, GameScreenData* _gameScreen)
{
	EnemyShooter* newEnemy = NULL;
	EnemyShooter_Initialize(&newEnemy, _gameScreen);
	Entity_MoveTo((Entity*)newEnemy, 
		_boss->mEntity.mPosition_x + MOUNTH_POS_X, 
		_boss->mEntity.mPosition_y + MOUNTH_POS_Y);
	DVectorPushBack(_gameScreen->mAllEntities, &newEnemy);
}

void Boss_OnCollide(Boss* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER_PROJECTILE:
		Entity_TakeDamages((Entity*)_current, 1);
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
