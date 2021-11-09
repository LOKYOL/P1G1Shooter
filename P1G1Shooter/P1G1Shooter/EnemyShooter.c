#include "EnemyShooter.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"
#include "projectile.h"
#include <math.h>

void EnemyShooter_Initialize(EnemyShooter** _enemy, 
	unsigned int _health, int _damage, int _speed,
	GameScreenData* _gameScreen)
{
	EnemyShooter* newEnemy = (EnemyShooter*)malloc(sizeof(EnemyShooter));
	memset(newEnemy, 0, sizeof(EnemyShooter));

	*_enemy = newEnemy;

	Entity_Initialize(&newEnemy->mEntity, _health, _damage, _speed, EnemyShooter_Update);

	newEnemy->mEntity.mDisplayZone = _gameScreen->mSprites[TYPE_ENEMY];

	newEnemy->mEntity.mDisplayZone.mPosY =
		rand() % (WINDOW_HEIGHT - newEnemy->mEntity.mDisplayZone.mSizeY);

	newEnemy->mEntity.mEntityType = TYPE_ENEMY;
	newEnemy->mEntity.mPosition_x = WINDOW_WIDTH;
	newEnemy->mEntity.mPosition_y = newEnemy->mEntity.mDisplayZone.mPosY;
	newEnemy->mShootCooldown = 5;
}

void EnemyShooter_Update(void* _enemy, Game* _game, GameScreenData* _gameScreen)
{
	EnemyShooter* myEnemy = (EnemyShooter*)_enemy;
	EnemyShooter_UpdateMovement(myEnemy, _gameScreen, _game);
	
	FlushDisplayZone(_game->mDisplaySettings, &myEnemy->mEntity.mDisplayZone);
}

void EnemyShooter_UpdateMovement(EnemyShooter* _enemy, GameScreenData* _gameScreen, Game* _game)
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
	
	//move_x = -1;
	
	// Aligning with player
	if (posEnemy_y < posPlayer_y)
	{
		move_y++;
	}
	else if(posEnemy_y > posPlayer_y)
	{
		move_y--;
	}

	// Not shooting until moved enough to the left
	if (posEnemy_x >= WINDOW_WIDTH - 5)
	{
		move_x--;
	}
	/*else
	{
		Enemy_Shoot(_enemy, _gameScreen);
	}*/

	// Clamp movement
	double movement = _enemy->mEntity.mSpeed * _game->mGameDt;
	double magnitude = sqrt(pow(move_x, 2) + pow(move_y, 2));
	move_x = move_x / magnitude * movement;
	move_y = move_y / magnitude * movement;

	// Apply movement
	Entity_Move(&_enemy->mEntity, move_x, move_y);
}

void Enemy_Shoot(EnemyShooter* _enemy, GameScreenData* _gameScreen)
{
	if (_enemy->mShootCooldown <= 0)
	{
		Projectile* newProjectile;
		Proj_Initialize(&newProjectile, 2, 1, _enemy->mEntity.mPosition_x, _enemy->mEntity.mPosition_y, TYPE_ENEMY_PROJECTILE, _gameScreen);

		PushEntity(_gameScreen, &newProjectile);

		_enemy->mShootCooldown = 5;
	}
	else
	{
		_enemy->mShootCooldown--;
	}
}