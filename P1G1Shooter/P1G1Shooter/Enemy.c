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
		3, 2, 1
	);

	DrawRectangleInDisplayZone
	(
		&newEnemy->mEntity.mDisplayZone,
		0, 0, 3, 2,
		WHITE, GREEN, ' '
	);


	newEnemy->mEntity.mEntityType = TYPE_ENEMY_KAMIKAZE;
	newEnemy->mEntity.mPosition_x = WINDOW_WIDTH;
	newEnemy->mEntity.mPosition_y = newEnemy->mEntity.mDisplayZone.mPosY;
}

void Enemy_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	Enemy* myEnemy = (Enemy*)_enemy;
	Enemy_UpdateMovement(myEnemy, _gameScreen, _game->mGameDt);
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void Enemy_UpdateMovement(Enemy* _enemy, GameScreenData* _gameScreen, double _deltaTime)
{
	double
		move_x = 0,
		move_y = 0;

	double
		posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x,
		posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y;
	
	move_x = -1;
	
	if (_enemy->mEntity.mPosition_y < posPlayer_y - 0.5)
	{
		move_y++;
	}
	else if(_enemy->mEntity.mPosition_y > posPlayer_y + 0.5)
	{
		move_y--;
	}

	double movement = _enemy->mEntity.mSpeed * _deltaTime;
	double magnitude = sqrt(pow(move_x, 2) + pow(move_y, 2));
	move_x = move_x / magnitude * movement;
	move_y = move_y / magnitude * movement;

	Entity_Move(&_enemy->mEntity, move_x, move_y);
}

void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen)
{

}