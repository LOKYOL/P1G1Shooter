#include "GameScreen.h"
#include "Game.h"
#include "TimeManagement.h"
#include "../PlayerStruct.h"
#include "../Obstacle.h"
#include "../Projectile.h"

int GameScreenInit(Game* game, GameState* state)
{
	return 0;
}

int GameScreenClose(Game* game, GameState* state)
{
	return 0;
}

int GameScreenUpdate(Game* game, GameState* state)
{
	// SPAWN OBSTACLE
	game->mGameSpawnObstacleTimer += game->mGameDt;
	if (game->mGameSpawnObstacleTimer >= OBSTACLE_SPAWN_TIMER)
	{
		game->mGameSpawnObstacleTimer -= OBSTACLE_SPAWN_TIMER;

		SpawnObstacle(game);
	}


	// FOR EACH ENTITY
	Entity* curEntity = NULL;
	for (int i = 0; i < game->mAllEntities->mCurrentSize; i++)
	{
		curEntity = *(Entity**)DVectorGet(game->mAllEntities, i);

		if (curEntity->mUpdate != NULL)
		{
			curEntity->mUpdate((void*)curEntity, game);
		}
	}

	// COLLISIONS
	DVector* PlayerList = GetAllEntityOfType(game, TYPE_PLAYER);
	DVector* ObstacleList = GetAllEntityOfType(game, TYPE_OBSTACLE);
	DVector* ProjectileList = GetAllEntityOfType(game, TYPE_PROJECTILE);

	Entity* curObstacle = NULL, * curProjectile = NULL;
	Player* player = *(Player**)DVectorGet(PlayerList, 0);

	if (player == NULL)
	{
		return 1;
	}

	// FOR EACH OBSTACLE
	for (int i = 0; i < ObstacleList->mCurrentSize; i++)
	{
		curObstacle = *(Obstacle**)DVectorGet(ObstacleList, i);

		// COMPARE COLLISION WITH THE PLAYER
		if (CompareCollision(curObstacle, &player->entity) > 0)
		{
			Entity_TakeDamages(player, curObstacle->mDamages);
			Entity_TakeDamages(curObstacle, INT_MAX);
			if (curObstacle->mHealth <= 0)
			{
				PopEntity(game, curObstacle);
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
					PopEntity(game, curObstacle);
					DVectorErase(ObstacleList, i);
					i--;
				}
				if (curProjectile->mHealth <= 0)
				{
					PopEntity(game, curProjectile);
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

		if (CompareCollision(curProjectile, &player->entity) > 0)
		{
			Entity_TakeDamages(player, curProjectile->mDamages);
			Entity_TakeDamages(curProjectile, INT_MAX);
			if (curProjectile->mHealth <= 0)
			{
				PopEntity(game, curProjectile);
				DVectorErase(ProjectileList, i);
				i--;
			}

		}
	}


	return 0;
}
