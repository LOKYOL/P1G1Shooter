#include "Enemy.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"

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


	newEnemy->mEntity.mEntityType = TYPE_ENEMY;
	newEnemy->mEntity.mPosition_x = WINDOW_WIDTH;
	newEnemy->mEntity.mPosition_y = newEnemy->mEntity.mDisplayZone.mPosY;
}

void Enemy_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	Enemy* myEnemy = (Enemy*)_enemy;
	Enemy_UpdateMovement(myEnemy, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void Enemy_UpdateMovement(Enemy* _enemy, GameScreenData* _gameScreen)
{
	double
		newpos_x = _enemy->mEntity.mPosition_x, 
		newpos_y = _enemy->mEntity.mPosition_y;

	double
		posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x,
		posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y;
	
	newpos_x = newpos_x + 0.4;
	
	if (newpos_y < posPlayer_y - 0.5)
	{
		newpos_y++;
	}
	else if(newpos_y > posPlayer_y + 0.5)
	{
		newpos_y--;
	}

	Entity_MoveTo(&_enemy->mEntity, newpos_x, newpos_y);

}

void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen)
{

}