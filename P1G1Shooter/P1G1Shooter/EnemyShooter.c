#include "EnemyShooter.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Player.h"
#include "projectile.h"
#include <math.h>

void EnemyShooter_Initialize(EnemyShooter** _enemy,	GameScreenData* _gameScreen)
{
	EnemyShooter* newEnemy = (EnemyShooter*)malloc(sizeof(EnemyShooter));
	memset(newEnemy, 0, sizeof(EnemyShooter));

	*_enemy = newEnemy;

	ParamSection* shooterSection = GetSection(_gameScreen->mParamsList, ENEMYS_INIT_SECTION);

	if(shooterSection)
	{
		ParamInt* shooterHealth = (ParamInt*)GetParamInSection(shooterSection, "Health_min");
		ParamInt* minShooterSpeed = (ParamInt*)GetParamInSection(shooterSection, "Speed_min");
		ParamInt* maxShooterSpeed = (ParamInt*)GetParamInSection(shooterSection, "Speed_max");
		ParamInt* ShootCooldown = (ParamInt*)GetParamInSection(shooterSection, "Shoot_cooldown");

		Entity_Initialize(&newEnemy->mEntity, TYPE_ENEMY_SHOOTER,
		WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - newEnemy->mEntity.mDisplayZone.mSizeY),
		shooterHealth->mValue, ((float)RandomInt(minShooterSpeed->mValue, maxShooterSpeed->mValue)),
		&_gameScreen->mSprites[TYPE_ENEMY_SHOOTER],
		EnemyShooter_Update, EnemyShooter_OnCollide, EnemyShooter_Destroy);
	
		newEnemy->mShootCooldown = ShootCooldown->mValue;
		newEnemy->mChangeDirectionCooldown = 0;
		newEnemy->mCurrentDirectionX = (rand() % 3) - 1;
		newEnemy->mCurrentDirectionY = (rand() % 3) - 1;
	}
}

void EnemyShooter_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	EnemyShooter* myEnemy = (EnemyShooter*)_enemy;
	EnemyShooter_UpdateMovement(myEnemy, _gameScreen, _game);
	
	if (myEnemy->mShootCooldown > 0)
	{
		myEnemy->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Enemy_Shoot(myEnemy, _gameScreen);
	}

	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void EnemyShooter_UpdateMovement(EnemyShooter* _enemy, GameScreenData* _gameScreen, Game* _game)
{
	double
		move_x = 0,
		move_y = 0;
	
	if (_enemy->mChangeDirectionCooldown > 0)
	{
		_enemy->mChangeDirectionCooldown -= _game->mGameDt;
	}
	else
	{
		_enemy->mChangeDirectionCooldown = 1;

		_enemy->mCurrentDirectionX = (rand() % 3) - 1;
		_enemy->mCurrentDirectionY = (rand() % 3) - 1;
	}

	move_y = _enemy->mCurrentDirectionY;
	move_x = _enemy->mCurrentDirectionX;

	// Not shooting until moved enough to the left
	if (_enemy->mEntity.mPosition_x < WINDOW_WIDTH / 2)
	{
		move_x += 2;
	}
	else if (_enemy->mEntity.mPosition_x > (double)(WINDOW_WIDTH / 5 * 4))
	{
		move_x -= 4;
	}

	// Clamp movement
	double movement = _enemy->mEntity.mSpeed * _game->mGameDt;
	double magnitude = sqrt((move_x * move_x) + (move_y * move_y));
	move_x = move_x / magnitude * movement;
	move_y = move_y / magnitude * movement;

	// Apply movement
	Entity_Move(&_enemy->mEntity, move_x, move_y);
}

void Enemy_Shoot(EnemyShooter* _enemy, GameScreenData* _gameScreen)
{
	Projectile* newProjectile;
	Proj_Initialize(&newProjectile, 40, 1, -1, 0,
		_enemy->mEntity.mPosition_x, _enemy->mEntity.mPosition_y,
		TYPE_ENEMY_PROJECTILE,TYPE_ENEMY_PROJECTILE, _gameScreen,
		Projectile_Movement_Standard,
		Projectile_Update, Projectile_OnCollide, Projectile_Destroy);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);

	_enemy->mShootCooldown = 5;
}

void EnemyShooter_OnCollide(EnemyShooter* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER_PROJECTILE:
		Entity_TakeDamages((Entity*)_current, 1);
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

void EnemyShooter_Destroy(Entity* _entity)
{
	free(_entity);
}
