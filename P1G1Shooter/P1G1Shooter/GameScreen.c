#include "GameScreen.h"
#include "Engine/Game.h"
#include "Engine/TimeManagement.h"
#include "PlayerStruct.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "Enemy.h"

const char CollisionsLayers[6] =
{
	//						P	O	PP	EP	E	EK
	//	Player				0	1	0	1	1	1
	//	Obstacles			1	0	1	1	0	1
	//	Player Projectiles	0	1	0	1	1	1
	//	Enemy Projectiles	1	1	1	0	0	1
	//	Enemy				1	0	1	0	0	0
	//	Enemy kamikaze		1	1	1	1	0	0

	0b010111,
	0b101101,
	0b010111,
	0b111001,
	0b101000,
	0b111100
};

int GameScreenInit(Game* game, GameState* state)
{
	state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = state->mData;

	data->mAllEntities = DVectorCreate();
	DVectorInit(data->mAllEntities, sizeof(Entity*), 0, NULL);

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer);
	data->mPlayer = myPlayer;

	data->mGameSpawnObstacleTimer = 0;
	data->mGameSpawnEnemyTimer = 0;

	return 0;
}

int GameScreenClose(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	DVectorDestroy(data->mAllEntities);
	return 0;
}

int GameScreenUpdate(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	// SPAWN OBSTACLE
	data->mGameSpawnObstacleTimer += game->mGameDt;
	if (data->mGameSpawnObstacleTimer >= OBSTACLE_SPAWN_TIMER)
	{
		data->mGameSpawnObstacleTimer -= OBSTACLE_SPAWN_TIMER;

		SpawnObstacle(data);
	}

	// SPAWN ENEMY
	data->mGameSpawnEnemyTimer += game->mGameDt;
	if (data->mGameSpawnEnemyTimer >= ENEMY_SPAWN_TIMER)
	{
		data->mGameSpawnEnemyTimer -= ENEMY_SPAWN_TIMER;

		SpawnEnemy(data);
	}

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
	}

	Entity* curObstacle = NULL, * curProjectile = NULL, * curEnemy = NULL;

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

	// COLLISIONS
	HandleCollision(data->mAllEntities);
	HandleEntityCollision(data->mPlayer, data->mAllEntities->mBuffer, data->mAllEntities->mCurrentSize);

	EndGame(game, data->mPlayer);

	for (int i = 0; i < data->mAllEntities->mCurrentSize; i++)
	{
		PopBackIfIsDead(data, DVectorGetTyped(data->mAllEntities, Entity*, i));
	}

	

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
			Entity_Free(curEntity);
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

void HandleCollision(DVector* _list)
{
	Entity* curEntity = NULL;
	for (int i = 0; i < (int)_list->mCurrentSize - 1; i++)
	{
		if ((curEntity = DVectorGetTyped(_list, Entity*, i)))
		{
			HandleEntityCollision(
				curEntity, 
				DVectorGet(_list, i + 1), 
				_list->mCurrentSize - i - 1
			);
		}
	}
}

void HandleEntityCollision(Entity* _entity, Entity** _list, int _length)
{
	Entity* curCompare = NULL;
	for (int i = 0; i < _length; i++)
	{
		if ((curCompare = _list[i])	&&
			CompareCollision(_entity, curCompare))
		{
			Entity_TakeDamages(_entity, curCompare->mDamages);
			Entity_TakeDamages(curCompare, _entity->mDamages);
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
		return layerA & (32 >> _entityB->mEntityType);
	}
	return 0;
}

void PopBackIfIsDead(GameScreenData* _game, Entity* _entity)
{
	if (Entity_IsDead(_entity))
	{
		PopEntity(_game, _entity);
	}
}

void SpawnObstacle(GameScreenData* _game)
{
	Obstacle* newObstacle = NULL;
	InitObstacle(&newObstacle);
	DVectorPushBack(_game->mAllEntities, &newObstacle);
}

void	SpawnEnemy(GameScreenData* _game)
{
	Enemy* newEnemy = NULL;
	InitEnemy(&newEnemy, 1, 1, (rand() % 10) + 40);
	DVectorPushBack(_game->mAllEntities, &newEnemy);
}

void EndGame(Game* _game, Player* _player)
{

	GameState	end;
	end.mStateInit = &EndScreenInit;
	end.mStateClose = &EndScreenClose;
	end.mStateUpdate = &EndScreenUpdate;

	if (Entity_IsDead(&_player->mEntity))
	{
		PushGameState(_game, end);
	}
}
