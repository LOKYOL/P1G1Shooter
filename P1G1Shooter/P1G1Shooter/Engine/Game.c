#include "Game.h"
#include "TimeManagement.h"
#include "TitleScreen.h"
#include "../PlayerStruct.h"
#include "../Obstacle.h"
#include "../Projectile.h"

void	InitGame(Game* game)
{
	game->mDisplaySettings = InitDisplay
	(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_FONT_SIZE,
		WINDOW_FONT_SQUARED
	);

	InitTime();

	game->mGameTime = GetTime();
	game->mGameDt = 0.0;


	game->mStateStack = DVectorCreate();
	DVectorInit(game->mStateStack, sizeof(GameState), 0, NULL);

	game->mAllEntities = DVectorCreate();
	DVectorInit(game->mAllEntities, sizeof(Entity*), 0, NULL);

	InitInputs(&game->mInputs);

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer);
	DVectorPushBack(game->mAllEntities, &myPlayer);

	GameState	title;
	title.mStateInit = &TitleScreenInit;
	title.mStateClose = &TitleScreenClose;
	title.mStateUpdate = &TitleScreenUpdate;

	PushGameState(game, title);

	game->mGameSpawnObstacleTimer = 0;
}

void	CloseGame(Game* game)
{
	// clear stack
	while (DVectorSize(game->mStateStack))
	{
		PopGameState(game);
	}

	DVectorDestroy(game->mStateStack);

	CloseDisplay(game->mDisplaySettings);

}

int		MainLoop(Game* game)
{
	double currentTime = GetTime();
	game->mGameDt = currentTime - game->mGameTime;
	game->mGameTime = currentTime;

	ClearBuffer(game->mDisplaySettings, WHITE, DARKER | BLUE);

	UpdateAllInputs(game->mInputs);

	int updateResult = 0;

	if (DVectorSize(game->mStateStack))
	{
		GameState current = (DVectorBackTyped(game->mStateStack, GameState));
		updateResult = current.mStateUpdate(game, &current);
	}

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

	Entity* curObstacle = NULL,* curProjectile = NULL;
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

	SwapBuffer(game->mDisplaySettings);

	return updateResult;
}

void	PushGameState(Game* game, GameState state)
{
	DVectorPushBack(game->mStateStack, &state);

	GameState* currentState = DVectorBack(game->mStateStack);
	currentState->mStateInit(game, currentState);

}

void	PopGameState(Game* game)
{
	if (DVectorSize(game->mStateStack))
	{
		GameState* currentState = DVectorBack(game->mStateStack);
		currentState->mStateClose(game, currentState);

		DVectorPopBack(game->mStateStack);
	}
}

void ChangeGameState(Game* _game, GameState _state)
{
	PopGameState(_game);
	PushGameState(_game, _state);
}

void SpawnObstacle(Game* _game)
{
	Obstacle* newObstacle = NULL;
	InitObstacle(&newObstacle);
	DVectorPushBack(_game->mAllEntities, &newObstacle);
}

void PushEntity(Game* _game, Entity** _entity)
{
	DVectorPushBack(_game->mAllEntities, _entity);
}

void PopEntity(Game* _game, Entity* _entity)
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

DVector* GetAllEntityOfType(Game* _game, EntityType _type)
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

void PopBackIfIsDead(Game* _game, Entity* _entity)
{
	if (Entity_IsDead(_entity))
	{
		PopEntity(_game, _entity);
		Entity_Free(_entity);
	}
}
