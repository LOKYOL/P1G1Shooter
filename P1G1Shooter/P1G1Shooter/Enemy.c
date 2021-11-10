#include "Enemy.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"
#include <math.h>

void Enemy_Initialize(Enemy** _enemy, GameScreenData* gameScreen)
{
	Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));
	memset(newEnemy, 0, sizeof(Enemy));

	*_enemy = newEnemy;

	int health = RandomInt(ENEMY_HEALTH_MIN, ENEMY_HEALTH_MAX),
		speed = RandomInt(ENEMY_SPEED_MIN, ENEMY_SPEED_MAX),
		damages = RandomInt(ENEMY_DAMAGES_MIN, ENEMY_DAMAGES_MAX);

	Entity_Initialize(&newEnemy->mEntity, health, damages, speed, Enemy_Update);

	newEnemy->mEntity.mDisplayZone = gameScreen->mSprites[TYPE_ENEMY_KAMIKAZE];

	newEnemy->mEntity.mDisplayZone.mPosY =
		rand() % (WINDOW_HEIGHT - newEnemy->mEntity.mDisplayZone.mSizeY);

	newEnemy->mEntity.mEntityType = TYPE_ENEMY_KAMIKAZE;
	newEnemy->mEntity.mPosition_x = WINDOW_WIDTH;
	newEnemy->mEntity.mPosition_y = newEnemy->mEntity.mDisplayZone.mPosY;
}

void Enemy_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	Enemy* myEnemy = (Enemy*)_enemy;
	Enemy_UpdateMovement(myEnemy, _gameScreen, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void Enemy_UpdateMovement(Enemy* _enemy, GameScreenData* _gameScreen, Game* _game)
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
	for (int i = 0; i < _gameScreen->mAllEntities->mCurrentSize; i++)
	{
		if ((curEntity = DVectorGetTyped(_gameScreen->mAllEntities, Entity*, i))	&&
				(curEntity->mEntityType == TYPE_OBSTACLE)					&&
			InRange(_enemy->mEntity.mDisplayZone.mPosY, 
			curEntity->mDisplayZone.mPosY - _enemy->mEntity.mDisplayZone.mSizeY - 2, 
			curEntity->mDisplayZone.mPosY + curEntity->mDisplayZone.mSizeY + 2)		&&
			(curDistance = Entity_GetDistance(_enemy, curEntity)) < minDistance)
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

	if (_enemy->mEntity.mPosition_x < -5 || _enemy->mEntity.mPosition_x > WINDOW_WIDTH + 5)
	{
		Entity_TakeDamages(_enemy, INT_MAX);
	}
}

int RandomInt(int min, int max)
{
	if (min < max)
	{
		return (rand() % (max - min + 1)) + min;
	}
	else
	{
		return min;
	}
}
