#include "GameScreen.h"
#include "Engine/TimeManagement.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Engine/ConsoleDisplay.h"
#include "PlayerStruct.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "Enemy.h"
#include "EnemyShooter.h"
#include "PowerupHealth.h"
#include "EndScreen.h"
#include <stdio.h>
#include "Engine/SoundManager.h"

#define NUM_OF_ENTITY_TYPES 7

const char CollisionsLayers[NUM_OF_ENTITY_TYPES] =
{
	//						P	O	PP	EP	E	EK	HPP
	//	Player				0	1	0	1	1	1	1
	//	Obstacles			1	0	1	1	0	1	0
	//	Player Projectiles	0	1	0	1	1	1	0
	//	Enemy Projectiles	1	1	1	0	0	1	0
	//	Enemy				1	0	1	0	0	0	0
	//	Enemy kamikaze		1	1	1	1	0	0	0
	//  Health PP			1	0	0	0	0	0	0

	0b0101111,
	0b1011010,
	0b0101110,
	0b1110010,
	0b1010000,
	0b1111000,
	0b1000000
};

int GameScreenInit(Game* game, GameState* state)
{
	state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = state->mData;
	
	data->mAllEntities = DVectorCreate();
	DVectorInit(data->mAllEntities, sizeof(Entity*), 0, 0);

	data->mSprites = (DisplayZone*)malloc(sizeof(DisplayZone) * NUM_OF_ENTITY_TYPES);

	DisplayZone* curDisplayZone = NULL;

	curDisplayZone = CreateDisplayZoneFromBMP("submarine.bmp");		// Player Sprite
	data->mSprites[0] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("sealion.bmp");	// OBstacles Sprite
	data->mSprites[1] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("bubulle.bmp");	// Player Projectile Sprite
	data->mSprites[2] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("bubulle.bmp");	// Enemy Projectile Sprite
	data->mSprites[3] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("enemy.bmp");		// Enemy Sprite
	data->mSprites[4] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("kamikaze_nrvtest.bmp");	// Enemy Kamikaze Sprite
	data->mSprites[5] = *curDisplayZone;
	free(curDisplayZone);
	curDisplayZone = CreateDisplayZoneFromBMP("health_pp.bmp");	// Health Powerup Sprite
	data->mSprites[6] = *curDisplayZone;
	free(curDisplayZone);

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer, data);
	data->mPlayer = myPlayer;

	data->mGameSpawnObstacleTimer = 0;
	data->mGameSpawnEnemyTimer = 0;
	data->mGameSpawnEnemyKamikazeTimer = 0;

	return 0;
}

int GameScreenClose(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	for (int i = 0; i < data->mAllEntities->mCurrentSize; i++)
	{
		free(DVectorGetTyped(data->mAllEntities, Entity*, i));
	}
	
	DVectorDestroy(data->mAllEntities);

	free(data->mPlayer);
	for (int i = 0; i < NUM_OF_ENTITY_TYPES; i++)
	{
		CloseDisplayZone(&data->mSprites[i]);
	}

	free(data->mSprites);

	free(state->mData);

	return 0;
}

int GameScreenUpdate(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	SpawnEntity(game, data);

	UpdateEntity(game, data);

	UpdateWeapon(game, data);

	// COLLISIONS
	HandleCollision(data->mAllEntities,	game);
	HandleEntityCollision(data->mPlayer, data->mAllEntities->mBuffer, data->mAllEntities->mCurrentSize, game);

	EndGame(game, data->mPlayer);
	
	FlushDisplayZone(game->mDisplaySettings, game->mScoreDisplayZone);

	return 0;
}

void PushEntity(GameScreenData* _game, Entity** _entity)
{
	DVectorPushBack(_game->mAllEntities, _entity);
}

void PopEntity(GameScreenData* _game, Entity* _entity)
{
	Entity* curEntity = NULL;
	for (int i = 0; i < _game->mAllEntities->mCurrentSize; i++)
	{
		if ((curEntity = DVectorGetTyped(_game->mAllEntities, Entity*, i)) == _entity)
		{
			free(_entity);
			DVectorErase(_game->mAllEntities, i);
			return;
		}
	}
}

DVector* GetAllEntityOfType(GameScreenData* _game, EntityType _type)
{
	DVector* list = DVectorCreate();
	DVectorInit(list, sizeof(void*), 0, 0);

	Entity* curEntity = NULL;
	for (int i = 0; i < _game->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(_game->mAllEntities, Entity*, i);

		if (curEntity->mEntityType == _type)
		{
			DVectorPushBack(list, &curEntity);
		}
	}

	return list;
}

void HandleCollision(DVector* _list, Game* gameStruct)
{
	Entity* curEntity = NULL;
	for (int i = 0; i < (int)_list->mCurrentSize - 1; i++)
	{
		if ((curEntity = DVectorGetTyped(_list, Entity*, i)))
		{
			HandleEntityCollision(
				curEntity, 
				DVectorGet(_list, i + 1), 
				_list->mCurrentSize - i - 1, 
				gameStruct
			);
		}
	}
}

void HandleEntityCollision(Entity* _entity, Entity** _list, int _length, Game* gameStruct)
{
	unsigned char mBool = 0;
	Entity* curCompare = NULL;
	for (int i = 0; i < _length; i++)
	{
		if ((curCompare = _list[i])	&&
			CompareCollision(_entity, curCompare))
		{
			Entity_TakeDamages(_entity, curCompare->mDamages);
			Entity_TakeDamages(curCompare, _entity->mDamages);

			if (_entity->mHealth > 0 && (_entity->mEntityType == TYPE_OBSTACLE || _entity->mEntityType == TYPE_ENEMY_KAMIKAZE)) 
			{
				Play_Sound("enemy_hit.wav", gameStruct->mSoundManager);
			} 
			else if (curCompare->mHealth > 0 && (curCompare->mEntityType == TYPE_OBSTACLE || curCompare->mEntityType == TYPE_ENEMY_KAMIKAZE)) 
			{
				Play_Sound("enemy_hit.wav", gameStruct->mSoundManager);
			}

			if (_entity->mEntityType == TYPE_PLAYER && _entity->mHealth > 0) 
			{
				Play_Sound("player_enemyhit.wav", gameStruct->mSoundManager);
			} 
			else if (curCompare->mEntityType == TYPE_PLAYER && curCompare->mHealth > 0) 
			{
				Play_Sound("player_enemyhit.wav", gameStruct->mSoundManager);
			}

			if (_entity->mEntityType == TYPE_PLAYER && _entity->mHealth == 0) 
			{
				Play_Sound("player_die.wav", gameStruct->mSoundManager);
			}
			else if (curCompare->mEntityType == TYPE_PLAYER && curCompare->mHealth == 0) 
			{
				Play_Sound("player_die.wav", gameStruct->mSoundManager);
			}

			if (_entity->mHealth == 0 && _entity->mEntityType == 2)
			{
				if (curCompare->mEntityType == 4)
				{
					//score += 3
					gameStruct->mScore += 3;
					mBool = 1;
				}
				else if (curCompare->mEntityType == 5)
				{
					//score += 4
					gameStruct->mScore += 4;
					mBool = 1;
				}
			}

			if (curCompare->mHealth == 0 && curCompare->mEntityType == 2) 
			{
				if (_entity->mEntityType == TYPE_ENEMY) 
				{
					gameStruct->mScore += 3;
					mBool = 1;
					//score += 3
				}
				else if (_entity->mEntityType == TYPE_ENEMY_KAMIKAZE)
				{
					gameStruct->mScore += 4;
					mBool = 1;
					//score += 4
				}
				else if (_entity->mEntityType == TYPE_OBSTACLE)
				{
					gameStruct->mScore += 1;
					mBool = 1;
				}
			}

			if (mBool) 
			{
				char totalScore[18] = "Score: ";

				snprintf(totalScore, 17, "Score: %d", gameStruct->mScore);

  				PrintInDisplayZone(&gameStruct->mScoreDisplayZone, WHITE, BLACK, 0, 0, totalScore, 0, NO_FLAG);
			}

		}
	}
}

char	CompareCollision(Entity* _entityA, Entity* _entityB)
{
	if (_entityA == _entityB)
	{
		return 0;
	}

	DisplayZone
		* zoneA = &_entityA->mDisplayZone, 
		* zoneB = &_entityB->mDisplayZone;

	return 
		CanCollide(_entityA, _entityB)		&&
		InRange(zoneA->mPosX, 
			zoneB->mPosX - zoneA->mSizeX, 
			zoneB->mPosX + zoneB->mSizeX)	&&
		InRange(zoneA->mPosY, 
			zoneB->mPosY - zoneA->mSizeY, 
			zoneB->mPosY + zoneB->mSizeY)
		
		/*zoneA->mPosX < zoneB->mPosX + zoneB->mSizeX &&
		zoneA->mPosX + zoneA->mSizeX > zoneB->mPosX &&
		zoneA->mPosY < zoneB->mPosY + zoneB->mSizeY &&
		zoneA->mPosY + zoneA->mSizeY > zoneB->mPosY*/;
}

char InRange(int value, int min, int max)
{
	return value < max && value > min;
}

char CanCollide(Entity* _entityA, Entity* _entityB)
{
	if (_entityA->mEntityType >= 0 && _entityB->mEntityType >= 0)
	{
		char	layerA = CollisionsLayers[_entityA->mEntityType];
		return layerA & ((1 << (NUM_OF_ENTITY_TYPES - 1)) >> _entityB->mEntityType);
	}
	return 0;
}

void SpawnEntity(Game* game, GameScreenData* _data)
{
	// SPAWN OBSTACLE
	_data->mGameSpawnObstacleTimer += game->mGameDt;
	if (_data->mGameSpawnObstacleTimer >= OBSTACLE_SPAWN_TIMER)
	{
		_data->mGameSpawnObstacleTimer -= OBSTACLE_SPAWN_TIMER;

		SpawnObstacle(_data);
	}

	// SPAWN ENEMY
	_data->mGameSpawnEnemyTimer += game->mGameDt;
	_data->mGameSpawnEnemyKamikazeTimer += game->mGameDt;

	if (_data->mGameSpawnEnemyTimer >= ENEMY_SPAWN_TIMER)
	{
		_data->mGameSpawnEnemyTimer -= ENEMY_SPAWN_TIMER;

		SpawnEnemy(_data);
	}

	if (_data->mGameSpawnEnemyKamikazeTimer >= ENEMY_KAMIKAZE_SPAWN_TIMER)
	{
		_data->mGameSpawnEnemyKamikazeTimer -= ENEMY_KAMIKAZE_SPAWN_TIMER;

		SpawnEnemyKamikaze(_data);
	}
}

void SpawnObstacle(GameScreenData* _game)
{
	Obstacle* newObstacle = NULL;
	Obstacle_Initialize(&newObstacle, _game);
	DVectorPushBack(_game->mAllEntities, &newObstacle);
}

void SpawnEnemy(GameScreenData* _game)
{
	EnemyShooter* newEnemyS = NULL;
	EnemyShooter_Initialize(&newEnemyS, 1, 1, (rand() % 10) + 40, _game);

	DVectorPushBack(_game->mAllEntities, &newEnemyS);
}

void SpawnEnemyKamikaze(GameScreenData* _game)
{
	Enemy* newEnemy = NULL;
	Enemy_Initialize(&newEnemy, _game);
	DVectorPushBack(_game->mAllEntities, &newEnemy);
}

void SpawnHealthPowerup(GameScreenData* _game, double posX, double posY)
{
	PowerupHealth* newPowerup = NULL;
	PowerupHealth_Initialize(&newPowerup, _game, posX, posY);
	DVectorPushBack(_game->mAllEntities, &newPowerup);
}

char PopBackIfIsDead(GameScreenData* _game, Entity* _entity, Game* gameStruct)
{
	char res = Entity_IsDead(_entity);
	if (res)
	{
		if (_entity->mEntityType == TYPE_OBSTACLE) 
		{
			Play_Sound("enemy_die.wav", gameStruct->mSoundManager);
		}
		else if (_entity->mEntityType == TYPE_ENEMY_KAMIKAZE || _entity->mEntityType == TYPE_ENEMY)
		{
			Play_Sound("enemy_die.wav", gameStruct->mSoundManager);
			if (RandomInt(1,20) == 20) // 5% de chance de spawner un bonus
			{
				SpawnHealthPowerup(_game, _entity->mPosition_x, _entity->mPosition_y);
			}
		}

		PopEntity(_game, _entity);
	}
	return res;
}

void UpdateEntity(Game* game, GameScreenData* data)
{
	data->mPlayer->mEntity.mUpdate(data->mPlayer, game, data);

	// FOR EACH ENTITY
	Entity* curEntity = NULL;
	for (int i = 0; i < data->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(data->mAllEntities, Entity*, i);

		if (curEntity->mUpdate != NULL)
		{
			curEntity->mUpdate((void*)curEntity, game, data);
		}

		if (PopBackIfIsDead(data, DVectorGetTyped(data->mAllEntities, Entity*, i), game))
		{
			i--;
		}
	}
}

void UpdateWeapon(Game* game, GameScreenData* data)
{
	// ENERGY RECHARGE
	if (data->mPlayer->mReloadCooldown > 0)
	{
		data->mPlayer->mReloadCooldown -= game->mGameDt;
	}
	else
	{
		data->mPlayer->mCurrentEnergy += game->mGameDt * RELOAD_SPEED;

		if (data->mPlayer->mCurrentEnergy >= MAX_ENERGY)
		{
			data->mPlayer->mCurrentEnergy = MAX_ENERGY;
		}
	}

	// SHOOT COOLDOWN
	if (data->mPlayer->mShootCooldown > 0)
	{
		data->mPlayer->mShootCooldown -= game->mGameDt;
	}
}

void EndGame(Game* _game, Player* _player)
{
	if (Entity_IsDead(&_player->mEntity))
	{
		PushEndScreen(_game);
	}
}
