#include "GameScreen.h"
#include "Engine/TimeManagement.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Engine/ConsoleDisplay.h"
#include "Player.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "EnemyKamikaze.h"
#include "EnemyShooter.h"
#include "PowerupHealth.h"
#include "EndScreen.h"
#include <stdio.h>
#include "Engine/SoundManager.h"

const char* spritesNames[NUM_OF_ENTITY_TYPES] =
{
	"Sprites/submarine.bmp",	// Player
	"Sprites/sealion.bmp",		// Obstacles
	"Sprites/bubulle.bmp",		// Player Projectiles
	"Sprites/bubulle.bmp",		// Enemies Projectiles
	"Sprites/enemy.bmp",		// Enemies
	"Sprites/kamikaze_nrv.bmp",	// Enemies kamikazes
	"Sprites/health_pp.bmp",	// PowerUp Health
	"Sprites/enemy.bmp"			// Boss
};

int GameScreenInit(Game* game, GameState* state)
{
	state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = state->mData;
	
	data->mAllEntities = DVectorCreate();
	DVectorInit(data->mAllEntities, sizeof(Entity*), 0, 0);

	data->mSprites = (DisplayZone*)malloc(sizeof(DisplayZone) * NUM_OF_ENTITY_TYPES);

	DisplayZone* curDisplayZone = NULL;
	for (int i = 0; i < NUM_OF_ENTITY_TYPES; i++)
	{
		curDisplayZone = CreateDisplayZoneFromBMP(spritesNames[i]);		// Player Sprite
		data->mSprites[i] = *curDisplayZone;
		free(curDisplayZone);
	}

	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer, data);
	data->mPlayer = myPlayer;

	data->mGameSpawnObstacleTimer = 0;
	data->mGameSpawnEnemyTimer = 0;
	data->mGameSpawnEnemyKamikazeTimer = 0;

	game->mScore = 0;
	data->mScoreDisplayZone = malloc(sizeof(DisplayZone));
	InitDisplayZone(data->mScoreDisplayZone, 0, 0, 20, 1, 0);

	return 0;
}

int GameScreenClose(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	Entity* curEntity = NULL;
	for (int i = 0; i < data->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(data->mAllEntities, Entity*, i);
		curEntity->mDestroy(curEntity);
	}
	DVectorDestroy(data->mAllEntities);

	data->mPlayer->mEntity.mDestroy(data->mPlayer);

	for (int i = 0; i < NUM_OF_ENTITY_TYPES; i++)
	{
		CloseDisplayZone(&data->mSprites[i]);
	}

	CloseDisplayZone(data->mScoreDisplayZone);
	free(data->mScoreDisplayZone);

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
	
	char totalScore[20] = "";
	snprintf(totalScore, 19, "Score : %d", game->mScore);
	PrintInDisplayZone(data->mScoreDisplayZone, WHITE, BLACK, 0, 0, totalScore, 0, NO_FLAG);

	EndGame(game, data->mPlayer);
	
	FlushDisplayZone(game->mDisplaySettings, data->mScoreDisplayZone);

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
			_entity->mDestroy(_entity);
			DVectorErase(_game->mAllEntities, i);
			return;
		}
	}
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
	Entity* curCompare = NULL;
	for (int i = 0; i < _length; i++)
	{
		if ((curCompare = _list[i])	&&
			CompareCollision(_entity, curCompare))
		{
			if (_entity->mOnCollide)
				_entity->mOnCollide(curCompare, gameStruct);

			if (curCompare->mOnCollide)
				curCompare->mOnCollide(_entity, gameStruct);

			// SCORE

			if (_entity->mHealth == 0 && _entity->mEntityType == TYPE_PLAYER_PROJECTILE)
			{
				if (curCompare->mEntityType == TYPE_ENEMY)
				{
					//score += 3
					gameStruct->mScore += 3;
				}
				else if (curCompare->mEntityType == TYPE_ENEMY_KAMIKAZE)
				{
					//score += 4
					gameStruct->mScore += 4;
				}
			}

			if (curCompare->mHealth == 0 && curCompare->mEntityType == TYPE_PLAYER_PROJECTILE) 
			{
				if (_entity->mEntityType == TYPE_ENEMY) 
				{
					gameStruct->mScore += 3;
					//score += 3
				}
				else if (_entity->mEntityType == TYPE_ENEMY_KAMIKAZE)
				{
					gameStruct->mScore += 4;
					//score += 4
				}
				else if (_entity->mEntityType == TYPE_OBSTACLE)
				{
					gameStruct->mScore += 1;
				}
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
	EnemyShooter_Initialize(&newEnemyS, _game);

	DVectorPushBack(_game->mAllEntities, &newEnemyS);
}

void SpawnEnemyKamikaze(GameScreenData* _game)
{
	EnemyKamikaze* newEnemy = NULL;
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
		if (/*_entity->mEntityType == TYPE_ENEMY_KAMIKAZE || */_entity->mEntityType == TYPE_ENEMY)
		{
			if (RandomInt(1,10) == 1) // 10% de chance de spawner un bonus
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
