#include "Enemy.h"

void InitEnemy(Enemy** enemy, double PosX, double PosY, unsigned int health, int damage, int speed)
{
	Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));
	Entity_Initialize(enemy, health, damage, speed, Enemy_Update);
	(*enemy)->mEntity.mEntityType = TYPE_ENEMY;
	(*enemy)->mEntity.mPosition_x = 4;
	(*enemy)->mEntity.mPosition_y = 4;
}

void Enemy_Update(void* _enemy, struct Game* _game, GameScreenData* _gameScreen)
{
	Enemy* myEnemy = (Enemy*)_enemy;	Enemy_UpdateMovement(myEnemy, _game);
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void Enemy_UpdateMovement(Enemy* _enemy, struct Game* _game)
{
	double
		newpos_x = _enemy->mEntity.mPosition_x, 
		newpos_y = _enemy->mEntity.mPosition_y;
}

void Enemy_Shoot(Enemy* enemy, GameScreenData* _gameScreen)
{

}
