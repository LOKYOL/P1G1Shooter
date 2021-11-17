#include "EnemyKamikaze.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Player.h"
#include <math.h>

void Enemy_Initialize(EnemyKamikaze** _enemy, GameScreenData* _gameScreen)
{
	EnemyKamikaze* newEnemy = (EnemyKamikaze*)malloc(sizeof(EnemyKamikaze));
	memset(newEnemy, 0, sizeof(EnemyKamikaze));

	*_enemy = newEnemy;

	float	health = (float)RandomInt(ENEMYK_HEALTH_MIN, ENEMYK_HEALTH_MAX),
			speed = (float)RandomInt(ENEMYK_SPEED_MIN, ENEMYK_SPEED_MAX);

	Entity_Initialize(&newEnemy->mEntity, TYPE_ENEMY_KAMIKAZE,
		WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - newEnemy->mEntity.mDisplayZone.mSizeY),
		health, speed, &_gameScreen->mSprites[TYPE_ENEMY_KAMIKAZE],
		Enemy_Update, Enemy_OnCollide, Enemy_Destroy);
}

void Enemy_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	EnemyKamikaze* myEnemy = (EnemyKamikaze*)_enemy;
	Enemy_UpdateMovement(myEnemy, _gameScreen, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void Enemy_UpdateMovement(EnemyKamikaze* _enemy, GameScreenData* _gameScreen, Game* _game)
{
	double
		move_x = 0,
		move_y = 0;

	int
		posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
		posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeY / 2) + 0.5,
		posEnemy_x = _enemy->mEntity.mPosition_x +
			(_enemy->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
		posEnemy_y = _enemy->mEntity.mPosition_y +
			(_enemy->mEntity.mDisplayZone.mSizeY / 2) + 0.5;
	
	move_x = -abs(posPlayer_x - posEnemy_x);
	move_y = (double)posPlayer_y - posEnemy_y;

	if (abs(move_y) > abs(move_x))
	{
		if (move_y > 0)
		{
			move_y = -move_x;
		}
		else
		{
			move_y = move_x;
		}
	}

	int minDistanceBase = _enemy->mEntity.mDisplayZone.mSizeX + 5;

	Entity* mostNear = NULL;
	double minDistance = minDistanceBase;
	Entity* curEntity = NULL;
	double curDistance = 0;
	for (unsigned int i = 0; i < _gameScreen->mAllEntities->mCurrentSize; i++)
	{
		if ((curEntity = DVectorGetTyped(_gameScreen->mAllEntities, Entity*, i))	&&
				(curEntity->mEntityType == TYPE_OBSTACLE)					&&
			InRange(_enemy->mEntity.mDisplayZone.mPosY, 
			curEntity->mDisplayZone.mPosY - _enemy->mEntity.mDisplayZone.mSizeY - 2, 
			curEntity->mDisplayZone.mPosY + curEntity->mDisplayZone.mSizeY + 2)		&&
			(curDistance = Entity_GetDistance(((Entity*)_enemy), curEntity)) < minDistance)
		{
			mostNear = curEntity;
			minDistance = curDistance;
		}
	}

	if (minDistance < minDistanceBase)
	{
		double height = mostNear->mPosition_y + (mostNear->mDisplayZone.mSizeY / 2);
		height -= _enemy->mEntity.mPosition_y + (_enemy->mEntity.mDisplayZone.mSizeY / 2);

		if (height < 0)
		{
			move_y = 1;
		}
		else
		{
			move_y = -1;
		}
	}

	// Clamp movement
	double movement = _enemy->mEntity.mSpeed * _game->mGameDt;
	double magnitude = sqrt(pow(move_x, 2) + pow(move_y, 2));
	move_x = move_x / magnitude * movement;
	move_y = move_y / magnitude * movement;

	// Apply movement
	Entity_Move(&_enemy->mEntity, move_x, move_y);

	if (_enemy->mEntity.mPosition_x < -5.f || _enemy->mEntity.mPosition_x > (double)(WINDOW_WIDTH + 5))
	{
		Entity_Kill((Entity*)_enemy);
	}
}

void Enemy_OnCollide(EnemyKamikaze* _current, Entity* _entity, Game* game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER:
	case TYPE_OBSTACLE:
	case TYPE_PLAYER_PROJECTILE:
	case TYPE_ENEMY_PROJECTILE:
		Entity_TakeDamages(_current, 1);
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

void Enemy_Destroy(Entity* _entity)
{
	free(_entity);
}
