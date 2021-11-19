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

	ParamSection* bossSection = GetSection(_gameScreen->mParamsList, BOSS_INIT_SECTION);

	if(bossSection)
	{
		ParamInt* bossHealth = (ParamInt*)GetParamInSection(bossSection, "Boss_health");
		ParamInt* bossPhaseBHealth = (ParamInt*)GetParamInSection(bossSection, "Boss_phase_b_health");
		ParamInt* bossSpeed = (ParamInt*)GetParamInSection(bossSection, "Boss_speed");
		ParamFloat* bossShootCooldown = (ParamFloat*)GetParamInSection(bossSection, "Boss_shoot_cooldown");
		ParamFloat* bossSpawnKamikazeTimer = (ParamFloat*)GetParamInSection(bossSection, "Spawn_kamikaze_timer");

		Entity_Initialize((Entity*)newBoss, TYPE_ENEMY_BOSS, 
			WINDOW_WIDTH, (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY) / 2, 
			bossHealth->mValue, bossSpeed->mValue,
			&_gameScreen->mSprites[TYPE_ENEMY_BOSS],
			Boss_Update, Boss_OnCollide, Boss_Destroy);

	newBoss->mCurrentPhaseUpdate = Boss_PhaseA_Update;
	newBoss->mCurrentMovementUpdate = Boss_Movement_EnterScreen;

	newBoss->mShootCooldown = bossShootCooldown->mValue;
	newBoss->mChangeDirectionCooldown = 1;
	newBoss->mCurrentDirectionX = RandomInt(-1, 1);
	newBoss->mCurrentDirectionY = RandomInt(-1, 1);
	newBoss->mHitTimer = 0;

	newBoss->mSpawnKamikazeCooldown = bossSpawnKamikazeTimer->mValue;
	newBoss->mCanSpawnKamikaze = 0;
	}
}

void Boss_Update(Boss* _boss, Game* _game, GameScreenData* _gameScreen)
{
	_boss->mCurrentPhaseUpdate(_boss, _game, _gameScreen);

	if (_boss)
	{
		

		_boss->mCurrentMovementUpdate(_boss, _game, _gameScreen);
		FlushDisplayZone(_game->mDisplaySettings, &_boss->mEntity.mDisplayZone);
	}
}

void Boss_PhaseA_Update(Boss* _boss, Game* _game, GameScreenData* _data)
{
	if (_boss->mShootCooldown > 0)
	{
		_boss->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Boss_Shoot(_boss, _data);
	}

	if (_boss->mEntity.mCurrentHealth <= BOSS_PHASE_B_HEALTH)
	{
		_boss->mCanSpawnKamikaze = 0;
		_boss->mCurrentPhaseUpdate = Boss_PhaseB_Update;
		_boss->mCurrentMovementUpdate = Boss_Movement_Dash;
	}

	if (_boss->mHitTimer > 0)
	{
		_boss->mHitTimer -= _game->mGameDt;

		// SPRITE HITTED
		if (_boss->mHitTimer <= 0)
		{
			_boss->mEntity.mDisplayZone.mBuffer =
				_data->mSprites[TYPE_ENEMY_BOSS].mBuffer;
		}
		else
		{
			_boss->mEntity.mDisplayZone.mBuffer =
				_data->mSprites[TYPE_ENEMY_BOSS + 1].mBuffer;
		}
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

	if (_boss->mCanSpawnKamikaze && _boss->mSpawnKamikazeCooldown <= 0)
	{
		_boss->mSpawnKamikazeCooldown -= ENEMY_KAMIKAZE_SPAWN_TIMER;

		Boss_Spawn_RandomEnemy(_boss, _data, _game);
	}
	else
	{
		_boss->mSpawnKamikazeCooldown -= _game->mGameDt;
	}

	if (_boss->mHitTimer > 0)
	{
		_boss->mHitTimer -= _game->mGameDt;

		// SPRITE HITTED
		if (_boss->mHitTimer <= 0)
		{
			_boss->mEntity.mDisplayZone.mBuffer = _data->mSprites[TYPE_ENEMY_BOSS + 2].mBuffer;
		}
		else
		{
			_boss->mEntity.mDisplayZone.mBuffer = _data->mSprites[TYPE_ENEMY_BOSS + 1].mBuffer;
		}
	}
}

void Boss_Spawn_RandomEnemy(Boss* _boss, GameScreenData* _data, Game* _game)
{
	if (rand() % 2)
	{
		Boss_SpawnKamikaze(_boss, _game, _data);
	}
	else
	{
		Boss_SpawnShooter(_boss, _game, _data);
	}
	_boss->mSpawnKamikazeCooldown = SPAWN_KAMIKAZE_TIMER;
}

void Boss_Movement_EnterScreen(Boss* _boss, Game* _game, GameScreenData* _data)
{
	if (_boss->mEntity.mPosition_x > WINDOW_WIDTH - _boss->mEntity.mDisplayZone.mSizeX - 5)
	{
		_boss->mCurrentDirectionX = -1;
	}
	else
	{
		_boss->mCurrentDirectionX = 0;
	}

	if (_boss->mEntity.mPosition_y > 0)
	{
		_boss->mCurrentDirectionY = -1;
	}
	else
	{
		_boss->mCurrentDirectionY = 0;
	}

	if (_boss->mCurrentDirectionX != 0 || _boss->mCurrentDirectionY != 0)
	{
		Entity_Move
		(
			(Entity*)_boss, 
			_boss->mCurrentDirectionX * _boss->mEntity.mSpeed * _game->mGameDt, 
			_boss->mCurrentDirectionY * _boss->mEntity.mSpeed * _game->mGameDt
		);
	}
	else
	{
		_boss->mCurrentMovementUpdate = Boss_Movement_UpDown;
		_boss->mCurrentDirectionX = 0;
		_boss->mCurrentDirectionY = -1;
	}
}

void Boss_Movement_Re_EnterScreen(Boss* _boss, Game* _game, GameScreenData* _data)
{
	if (_boss->mEntity.mPosition_x > WINDOW_WIDTH - _boss->mEntity.mDisplayZone.mSizeX - 5)
	{
		Entity_Move((Entity*)_boss, -_boss->mEntity.mSpeed * _game->mGameDt, 0);
	}
	else
	{
		_boss->mCurrentMovementUpdate = Boss_Movement_UpDown;
		_boss->mCurrentDirectionX = 0;
		_boss->mCurrentDirectionY = -1;
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
	_boss->mShootCooldown = BOSS_SHOOT_COOLDOWN;

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

void Boss_Shoot(Boss* _boss, GameScreenData* _gameScreen)
{
	Projectile* newProjectile;

	if (rand() % 2)
	{
		Boss_Shoot_LeftEye(_boss, _gameScreen);
	}

	if (rand() % 2)
	{
		Boss_Shoot_RightEye(_boss, _gameScreen);
	}

	_boss->mShootCooldown = BOSS_SHOOT_COOLDOWN;
}

void Boss_Shoot_RightEye(Boss* _boss, GameScreenData* _data)
{
	Projectile* newProjectile;

	double
		dir_x = -1,
		dir_y = 0;

	Proj_Initialize(&newProjectile, 1,
		(float)dir_x, (float)dir_y,
		_boss->mEntity.mPosition_x + EYE_RIGHT_POS_X,
		_boss->mEntity.mPosition_y + EYE_RIGHT_POS_Y,
		TYPE_ENEMY_PROJECTILE, TYPE_ENEMY_PROJECTILE, _data,
		Projectile_Movement_Standard,
		Projectile_Update, Projectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_data->mAllEntities, &newProjectile);
}

void Boss_Shoot_LeftEye(Boss* _boss, GameScreenData* _data)
{
	Projectile* newProjectile;

	double
		dir_x = _data->mPlayer->mEntity.mPosition_x -
		(_boss->mEntity.mPosition_x + EYE_LEFT_POS_X),
		dir_y = _data->mPlayer->mEntity.mPosition_y -
		(_boss->mEntity.mPosition_y + EYE_LEFT_POS_Y);

	float magnitude = sqrt((dir_x * dir_x) + (dir_y * dir_y));
	dir_x /= magnitude;
	dir_y /= magnitude;

	Proj_Initialize(&newProjectile, 1,
		(float)dir_x, (float)dir_y,
		_boss->mEntity.mPosition_x + EYE_LEFT_POS_X,
		_boss->mEntity.mPosition_y + EYE_LEFT_POS_Y,
		TYPE_ENEMY_PROJECTILE, TYPE_ENEMY_PROJECTILE, _data,
		Projectile_Movement_Standard,
		Projectile_Update, Projectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_data->mAllEntities, &newProjectile);
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

void Boss_OnCollide(Boss* _current, Entity* _entity, Game* _game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER_PROJECTILE:
		Entity_TakeDamages((Entity*)_current, 1);
		_current->mHitTimer = 0.5;
		if (_current->mEntity.mCurrentHealth > 0)
		{
			Play_Sound("enemy_hit", _game->mSoundManager);
		}
		else
		{
			Play_Sound("enemy_die", _game->mSoundManager);
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
