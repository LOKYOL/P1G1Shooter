#include "Boss.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"
#include "PlayerStruct.h"
#include "Projectile.h"
#include <math.h>

void Boss_Initialize(Boss** _boss,
	unsigned int _health, int _damage, int _speed,
	GameScreenData* _gameScreen)
{
	Boss* newBoss = (Boss*)malloc(sizeof(Boss));
	memset(newBoss, 0, sizeof(Boss));

	*_boss = newBoss;

	Entity_Initialize(&newBoss->mEntity, _health, _damage, _speed, Boss_Update);

	newBoss->mEntity.mDisplayZone = _gameScreen->mSprites[TYPE_BOSS];

	newBoss->mEntity.mDisplayZone.mPosY = WINDOW_HEIGHT - (newBoss->mEntity.mDisplayZone.mSizeY / 2);
		//rand() % (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY);

	newBoss->mEntity.mEntityType = TYPE_BOSS;
	newBoss->mEntity.mPosition_x = WINDOW_WIDTH;
	newBoss->mEntity.mPosition_y = newBoss->mEntity.mDisplayZone.mPosY;
	newBoss->mShootCooldown = 2;
	newBoss->mChangeDirectionCooldown = 1;
	newBoss->mCurrentDirectionX = (rand() % 3) - 1;
	newBoss->mCurrentDirectionY = (rand() % 3) - 1;
}

void Boss_Update(void* _boss, Game* _game, GameScreenData* _gameScreen)
{
	Boss* myBoss = (Boss*)_boss;
	Boss_UpdateMovement(myBoss, _gameScreen, _game);

	if (myBoss->mShootCooldown > 0)
	{
		myBoss->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Enemy_Shoot(myBoss, _gameScreen);
	}

	FlushDisplayZone(_game->mDisplaySettings, &myBoss->mEntity.mDisplayZone);
}

void Boss_UpdateMovement(Boss* _boss, GameScreenData* _gameScreen, Game* _game)
{
	double
		move_x = 0,
		move_y = 0;

	int
		posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x +
		(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
		posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y +
		(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeY / 2) + 0.5,
		posEnemy_x = _boss->mEntity.mPosition_x +
		(_boss->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
		posEnemy_y = _boss->mEntity.mPosition_y +
		(_boss->mEntity.mDisplayZone.mSizeY / 2) + 0.5;

	if (_boss->mChangeDirectionCooldown > 0)
	{
		_boss->mChangeDirectionCooldown -= _game->mGameDt;
	}
	else
	{
		_boss->mChangeDirectionCooldown = 1;

		_boss->mCurrentDirectionX = (rand() % 3) - 1;
		_boss->mCurrentDirectionY = (rand() % 3) - 1;
	}

	move_y = _boss->mCurrentDirectionY;
	move_x = _boss->mCurrentDirectionX;

	// Not shooting until moved enough to the left
	if (posEnemy_x < WINDOW_WIDTH / 2)
	{
		move_x += 2;
	}
	else if (posEnemy_x > WINDOW_WIDTH / 5 * 4)
	{
		move_x -= 4;
	}

	// Clamp movement
	double movement = _boss->mEntity.mSpeed * _game->mGameDt;
	double magnitude = sqrt(pow(move_x, 2) + pow(move_y, 2));
	move_x = move_x / magnitude * movement;
	move_y = move_y / magnitude * movement;

	// Apply movement
	Entity_Move(&_boss->mEntity, move_x, move_y);
}

void Boss_Shoot(Boss* _boss, GameScreenData* _gameScreen)
{
	Projectile* newProjectile;
	Proj_Initialize(&newProjectile, 2, 0,
		_boss->mEntity.mPosition_x, _boss->mEntity.mPosition_y,
		TYPE_ENEMY_PROJECTILE, _gameScreen);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);

	_boss->mShootCooldown = 5;
}