#include "GameScreen.h"
#include "Game.h"
#include "TimeManagement.h"
#include "../PlayerStruct.h"
#include "../Obstacle.h"
#include "../Projectile.h"
#include "../Enemy.h"

int GameScreenInit(Game* game, GameState* state)
{
	state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = state->mData;

	data->mAllEntities = DVectorCreate();
	DVectorInit(data->mAllEntities, sizeof(Entity*), 0, NULL);

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer);
	DVectorPushBack(data->mAllEntities, &myPlayer);

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

	// FOR EACH ENTITY
	Entity* curEntity = NULL;
	for (int i = 0; i < data->mAllEntities->mCurrentSize; i++)
	{
		curEntity = *(Entity**)DVectorGet(data->mAllEntities, i);

		if (curEntity->mUpdate != NULL)
		{
			curEntity->mUpdate((void*)curEntity, game, data);
		}
	}

	// COLLISIONS
	DVector* PlayerList = GetAllEntityOfType(data, TYPE_PLAYER);
	DVector* ObstacleList = GetAllEntityOfType(data, TYPE_OBSTACLE);
	DVector* ProjectileList = GetAllEntityOfType(data, TYPE_PROJECTILE);

	Entity* curObstacle = NULL, * curProjectile = NULL;
	Player* player = *(Player**)DVectorGet(PlayerList, 0);

	// ENERGY RECHARGE
	if (player->mReloadCooldown > 0)
	{
		player->mReloadCooldown -= game->mGameDt;
	}
	else
	{
		player->mCurrentEnergy += game->mGameDt * RELOAD_SPEED;

		if (player->mCurrentEnergy >= MAX_ENERGY)
		{
			player->mCurrentEnergy = MAX_ENERGY;
		}
	}

	// SHOOT COOLDOWN
	if (player->mShootCooldown > 0)
	{
		player->mShootCooldown -= game->mGameDt;
	}

	// FOR EACH OBSTACLE
	for (int i = 0; i < ObstacleList->mCurrentSize; i++)
	{
		curObstacle = *(Obstacle**)DVectorGet(ObstacleList, i);

		// COMPARE COLLISION WITH THE PLAYER
		if (CompareCollision(curObstacle, &player->mEntity) > 0)
		{
			Entity_TakeDamages(player, curObstacle->mDamages);
			Entity_TakeDamages(curObstacle, INT_MAX);
			if (curObstacle->mHealth <= 0)
			{
				PopEntity(data, curObstacle);
				DVectorErase(ObstacleList, i);
				i--;
			}
		}

		// COMPARE COLLISION WITH EACH PROJECTILE
		for (int j = 0; j < ProjectileList->mCurrentSize; j++)
		{
			curProjectile = *(Projectile**)DVectorGet(ProjectileList, j);
			if (CompareCollision(curObstacle, curProjectile) > 0)
			{
				Entity_TakeDamages(curObstacle, curProjectile->mDamages);
				Entity_TakeDamages(curProjectile, curObstacle->mDamages);
				if (curObstacle->mHealth <= 0)
				{
					PopEntity(data, curObstacle);
					DVectorErase(ObstacleList, i);
					i--;
				}
				if (curProjectile->mHealth <= 0)
				{
					PopEntity(data, curProjectile);
					DVectorErase(ProjectileList, j);
					j--;
				}
			}
		}
	}

	// FOREACH PROJECTILE
	for (int i = 0; i < ProjectileList->mCurrentSize; i++)
	{
		curProjectile = *(Projectile**)DVectorGet(ProjectileList, i);

		if (CompareCollision(curProjectile, &player->mEntity) > 0)
		{
			Entity_TakeDamages(player, curProjectile->mDamages);
			Entity_TakeDamages(curProjectile, INT_MAX);
			if (curProjectile->mHealth <= 0)
			{
				PopEntity(data, curProjectile);
				DVectorErase(ProjectileList, i);
				i--;
			}
		}
	}

	free(PlayerList);
	free(ObstacleList);
	free(ProjectileList);

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
		if ((curEntity = *(Entity**)DVectorGet(_game->mAllEntities, i)) == _entity)
		{
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
		curEntity = *(Entity**)DVectorGet(_game->mAllEntities, i);

		if (curEntity->mEntityType == _type)
		{
			DVectorPushBack(list, &curEntity);
		}
	}

	return list;
}

char	CompareCollision(Entity* _entityA, Entity* _entityB)
{
	DisplayZone* zoneA = &_entityA->mDisplayZone, * zoneB = &_entityB->mDisplayZone;

	if (zoneA->mPosX < zoneB->mPosX + zoneB->mSizeX &&
		zoneA->mPosX + zoneA->mSizeX > zoneB->mPosX &&
		zoneA->mPosY < zoneB->mPosY + zoneB->mSizeY &&
		zoneA->mPosY + zoneA->mSizeY > zoneB->mPosY)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void PopBackIfIsDead(GameScreenData* _game, Entity* _entity)
{
	if (Entity_IsDead(_entity))
	{
		PopEntity(_game, _entity);
		Entity_Free(_entity);
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
	InitEnemy(&newEnemy, 1, 1, (rand() % 5) + 3);
	DVectorPushBack(_game->mAllEntities, &newEnemy);
}
