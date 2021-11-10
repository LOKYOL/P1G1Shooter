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

	newBoss->mEntity.mDisplayZone.mPosY = (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY) / 2;
		//rand() % (WINDOW_HEIGHT - newBoss->mEntity.mDisplayZone.mSizeY);

	newBoss->mEntity.mEntityType = TYPE_BOSS;
	newBoss->mEntity.mPosition_x = WINDOW_WIDTH;
	newBoss->mEntity.mPosition_y = newBoss->mEntity.mDisplayZone.mPosY;
	newBoss->mShootCooldown = 2;
	newBoss->mContactAttackCooldown = 5;
	newBoss->mChangeDirectionCooldown = 1;
	newBoss->mTargetedPosX = 0;
	newBoss->mTargetedPosY = 0;
	newBoss->mCurrentDirectionX = (rand() % 3) - 1;
	newBoss->mCurrentDirectionY = (rand() % 3) - 1;
	newBoss->mContactAttackStatus = NOT_LAUNCHED;
}

void Boss_Update(void* _boss, Game* _game, GameScreenData* _gameScreen)
{
	Boss* myBoss = (Boss*)_boss;

	Boss_UpdateMovement(myBoss, _gameScreen, _game);

	Boss_ContactAttack(_boss, _gameScreen);

	if (myBoss->mShootCooldown > 0)
	{
		myBoss->mShootCooldown -= _game->mGameDt;
	}
	else
	{
		Boss_Shoot(myBoss, _gameScreen);
	}

	FlushDisplayZone(_game->mDisplaySettings, &myBoss->mEntity.mDisplayZone);
}

void Boss_UpdateMovement(Boss* _boss, GameScreenData* _gameScreen, Game* _game)
{
	double
		move_x = 0,
		move_y = 0;

	if (_boss->mContactAttackStatus == ATTACKING)					// Go to targeted position
	{
		if (_boss->mEntity.mPosition_x > _boss->mTargetedPosX)
		{
			move_x--;
		}
		else
		{
			move_x++;
		}
		if (_boss->mEntity.mPosition_y > _boss->mTargetedPosY)
		{
			move_y--;
		}
		else
		{
			move_y++;
		}
	}
	else if (_boss->mContactAttackStatus == RETURNING_FROM_ATTACK)	// Return to spawn position
	{
		if (_boss->mEntity.mPosition_x < WINDOW_WIDTH - _boss->mEntity.mDisplayZone.mSizeX)
		{
			move_x++;
		}
		if (_boss->mEntity.mPosition_y > (WINDOW_HEIGHT - _boss->mEntity.mDisplayZone.mSizeY) / 2)
		{
			move_y--;
		}
		else
		{
			move_y++;
		}
	}
	else	// Regular pattern to define
	{
		int
			posPlayer_x = _gameScreen->mPlayer->mEntity.mPosition_x +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
			posPlayer_y = _gameScreen->mPlayer->mEntity.mPosition_y +
			(_gameScreen->mPlayer->mEntity.mDisplayZone.mSizeY / 2) + 0.5,
			posEnemy_x = _boss->mEntity.mPosition_x +
			(_boss->mEntity.mDisplayZone.mSizeX / 2) + 0.5,
			posEnemy_y = _boss->mEntity.mPosition_y +
			(_boss->mEntity.mDisplayZone.mSizeY / 2) + 0.5;
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
	Proj_Initialize(&newProjectile, 3, 0,
		_boss->mEntity.mPosition_x, _boss->mEntity.mPosition_y,
		TYPE_ENEMY_PROJECTILE, _gameScreen);

	DVectorPushBack(_gameScreen->mAllEntities, &newProjectile);

	_boss->mShootCooldown = 5;
}

// Launch attack to targeted position (current player position)
void Boss_LaunchContactAttack(Boss* _boss, GameScreenData* _gameScreen)
{
	_boss->mContactAttackStatus = ATTACKING;
	_boss->mTargetedPosX = _gameScreen->mPlayer->mEntity.mPosition_x;
	_boss->mTargetedPosY = _gameScreen->mPlayer->mEntity.mPosition_y;
}

void Boss_ContactAttack(Boss* _boss, GameScreenData* _gameScreen, Game* _game)
{
	Entity* targetedPos;
	targetedPos->mPosition_x = _boss->mTargetedPosX;
	targetedPos->mPosition_y = _boss->mTargetedPosY;

	if (_boss->mContactAttackCooldown > 0)
	{
		_boss->mContactAttackCooldown -= _game->mGameDt;
	}
	else if (_boss->mContactAttackCooldown != -1)
	{
		Boss_LaunchContactAttack(_boss, _gameScreen);
	}

	if (_boss->mContactAttackStatus == ATTACKING)
	{
		if (Entity_GetDistance(_boss, targetedPos) < 1)
		{
			if (Entity_GetDistance(_boss, _gameScreen->mPlayer) < 1)
			{
				Entity_TakeDamages(_gameScreen->mPlayer, 1);
			}
			_boss->mContactAttackStatus == RETURNING_FROM_ATTACK;
			_boss->mTargetedPosX = WINDOW_WIDTH - _boss->mEntity.mDisplayZone.mSizeX;
			_boss->mTargetedPosY = (WINDOW_HEIGHT - _boss->mEntity.mDisplayZone.mSizeY) / 2;
			_boss->mContactAttackCooldown = -1;
		}
	}
	if (_boss->mContactAttackStatus == RETURNING_FROM_ATTACK)
	{
		if (Entity_GetDistance(_boss, targetedPos) < 1)
		{
			_boss->mContactAttackStatus == NOT_LAUNCHED;
			_boss->mTargetedPosX = 0;
			_boss->mTargetedPosY = 0;
			_boss->mContactAttackCooldown = 5;
		}
	}
}