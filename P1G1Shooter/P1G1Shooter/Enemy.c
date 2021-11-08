#include "Enemy.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"
#include <math.h>

void InitEnemy(Enemy** _enemy, unsigned int _health, int _damage, int _speed)
{
	Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));
	memset(newEnemy, 0, sizeof(Enemy));

	*_enemy = newEnemy;

	Entity_Initialize(&newEnemy->mEntity, _health, _damage, _speed, Enemy_Update);

	InitDisplayZone // TEMPORARY
	(
		&newEnemy->mEntity.mDisplayZone,
		WINDOW_WIDTH,
		(rand() % (WINDOW_HEIGHT - 2)) + 1,
		30, 20, 1
	);

	/*DrawRectangleInDisplayZone
	(
		&newEnemy->mEntity.mDisplayZone,
		0, 0, 3, 2,
		WHITE, GREEN, ' '
	);*/

	newEnemy->mEntity.mDisplayZone = *(CreateDisplayZoneFromBMP("kamikaze_nrvtest.bmp"));


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

	double
		posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeX / 2),
		posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeY / 2),
		posEnemy_x = _enemy->mEntity.mPosition_x +
			(_enemy->mEntity.mDisplayZone.mSizeX / 2),
		posEnemy_y = _enemy->mEntity.mPosition_y +
			(_enemy->mEntity.mDisplayZone.mSizeY / 2);
	
	move_x = -1;
	
	if (posEnemy_y < posPlayer_y)
	{
		move_y++;
	}
	else if(posEnemy_y > posPlayer_y)
	{
		move_y--;
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
}

void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen)
{

}