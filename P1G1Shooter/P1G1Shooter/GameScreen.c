#include "GameScreen.h"
#include "Engine/TimeManagement.h"
#include "Engine/DisplayZoneDrawing.h"
#include "Engine/ConsoleDisplay.h"
#include "Boss.h"
#include "Player.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "EnemyKamikaze.h"
#include "EnemyShooter.h"
#include "PowerupHealth.h"
#include "EndScreen.h"
#include "Engine/SoundManager.h"
#include <stdio.h>
#include "PowerupAimAssist.h"


int GameScreenInit(Game* game, GameState* state)
{
	state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = state->mData;
	
	data->mAllEntities = DVectorCreate();
	DVectorInit(data->mAllEntities, sizeof(Entity*), ZERO, ZERO);

	data->mParamsList = InitParamListFromIniFile(INI_PATH);

	LoadSpritesFromIni(data);
	
	// Create Player
	Player* myPlayer;
	InitPlayer(&myPlayer, data);
	data->mPlayer = myPlayer;
	data->mNextBossScore = 50; // Default value

	ParamSection* gameSection = GetSection(data->mParamsList, GAMESCREEN_INIT_SECTION);

	if (gameSection) {
		ParamInt* bossScore = (ParamInt*)GetParamInSection(gameSection, "Boss_score");
		data->mNextBossScore = bossScore->mValue;

	}

	data->mGameSpawnObstacleTimer = ZERO;
	data->mGameSpawnEnemyTimer = ZERO;
	data->mGameSpawnEnemyKamikazeTimer = ZERO;

	game->mScore = ZERO;
	data->mScoreDisplayZone = malloc(sizeof(DisplayZone));
	InitDisplayZone(data->mScoreDisplayZone, ZERO, ZERO, 20, 1, ZERO);

	return 0;
}

void LoadSpritesFromIni(GameScreenData* data)
{
	ParamSection* spritesSection = GetSection(data->mParamsList, "Sprites");

	if (spritesSection) {
		ParamInt* spritesSize = (ParamInt*)GetParamInSection(spritesSection, "SIZEOF_SPRITES_NAMES");

		if (spritesSize) {
			data->mSprites = (DisplayZone*)malloc(sizeof(DisplayZone) * spritesSize->mValue);

			DisplayZone* curDisplayZone = NULL;
			char* path = (char*)malloc(sizeof(char) * 200);
			char* spriteParam = (char*)malloc(sizeof(char) * 10);

			for (int i = 0; i < spritesSize->mValue; i++)
			{
				snprintf(spriteParam, 10, "Sprite_%d", i);
				GetParamElemString(spritesSection, path, 200, spriteParam);
				curDisplayZone = CreateDisplayZoneFromBMP(path);
				data->mSprites[i] = *curDisplayZone;
				free(curDisplayZone); 
			}

			free(spriteParam);
			free(path);
		}
	}
}

int GameScreenClose(Game* game, GameState* state)
{
	GameScreenData* data = state->mData;

	Entity* curEntity = NULL;
	for (unsigned int i = ZERO; i < data->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(data->mAllEntities, Entity*, i);
		curEntity->mDestroy(curEntity);
	}
	DVectorDestroy(data->mAllEntities);

	data->mPlayer->mEntity.mDestroy(data->mPlayer);


	ParamSection* spritesSection = GetSection(data->mParamsList, "Sprites");

	if (spritesSection) {
		ParamInt* spritesSize = (ParamInt*)GetParamInSection(spritesSection, "SIZEOF_SPRITES_NAMES");

		for (int i = ZERO; i < spritesSize->mValue; i++)
		{
			CloseDisplayZone(&data->mSprites[i]);
		}
	}
	

	CloseDisplayZone(data->mScoreDisplayZone);
	free(data->mScoreDisplayZone);

	free(data->mSprites);

	ClearParamList(data->mParamsList);
	free(data->mParamsList);

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
	HandleEntityCollision((Entity*)data->mPlayer, data->mAllEntities->mBuffer, data->mAllEntities->mCurrentSize, game);

	if (game->mScore >= data->mNextBossScore)
	{
		PushBossScreen(game);
	}

	char totalScore[20] = "";
	snprintf(totalScore, 19, "Score : %d", game->mScore);
	PrintInDisplayZone(data->mScoreDisplayZone, WHITE, BLACK, ZERO, ZERO, totalScore, ZERO, NO_FLAG);

	EndGame(game, data);
	
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
	for (unsigned int i = ZERO; i < _game->mAllEntities->mCurrentSize; i++)
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
	for (int i = ZERO; i < (int)_list->mCurrentSize - 1; i++)
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
	for (int i = ZERO; i < _length; i++)
	{
		HandleEntitiesCollision(_entity, _list[i], gameStruct);
	}
}

void HandleEntitiesCollision(Entity* _entityA, Entity* _entityB, Game* _game)
{
	if (_entityB && CompareCollision(_entityA, _entityB))
	{
		if (_entityA->mOnCollide)
			_entityA->mOnCollide(_entityA, _entityB, _game);

		if (_entityB->mOnCollide)
			_entityB->mOnCollide(_entityB, _entityA, _game);

		HandleScore(_entityA, _entityB, _game);
	}
}

void HandleScore(Entity* _entityA, Entity* _entityB, Game* _game)
{
	if (_entityB->mCurrentHealth == 0 && _entityB->mEntityType == TYPE_PLAYER_PROJECTILE)
	{
		_game->mScore += GetScoreOfEntity(_entityA);
	}
	else if (_entityA->mCurrentHealth == 0 && _entityA->mEntityType == TYPE_PLAYER_PROJECTILE)
	{
		_game->mScore += GetScoreOfEntity(_entityB);
	}
}

unsigned int GetScoreOfEntity(Entity* _entity)
{
	EntityType type = _entity->mEntityType;

	switch (_entity->mEntityType)
	{
	case TYPE_OBSTACLE: return 1;
	case TYPE_ENEMY_SHOOTER: return 3;
	case TYPE_ENEMY_KAMIKAZE: return 4;
	default: return 0;
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

void SpawnAimAssistPowerup(GameScreenData* _game, double posX, double posY)
{
	PowerupAimAssist* newPowerup = NULL;
	PowerupAimAssist_Initialize(&newPowerup, _game, posX, posY);
	DVectorPushBack(_game->mAllEntities, &newPowerup);
}

char PopBackIfIsDead(GameScreenData* _game, Entity* _entity, Game* gameStruct)
{
	char res = Entity_IsDead(_entity);
	if (res)
	{
		if (_entity->mEntityType == TYPE_ENEMY_SHOOTER)
		{
			if (RandomInt(1, 10) == 1) // 10% de chance de spawner un bonus
			{
				SpawnHealthPowerup(_game, _entity->mPosition_x, _entity->mPosition_y);
			}
		}
		else if (_entity->mEntityType == TYPE_ENEMY_KAMIKAZE)
		{
			if (RandomInt(1, 15) == 1) // 10% de chance de spawner un bonus
			{
				SpawnAimAssistPowerup(_game, _entity->mPosition_x, _entity->mPosition_y);
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
	for (unsigned int i = 0; i < data->mAllEntities->mCurrentSize; i++)
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
	if (data->mPlayer->mCurrentReloadCooldown > ZERO)
	{
		data->mPlayer->mCurrentReloadCooldown -= game->mGameDt;
	}
	else
	{
		data->mPlayer->mCurrentEnergy += (float)(game->mGameDt * data->mPlayer->mReloadGain);

		if (data->mPlayer->mCurrentEnergy >= data->mPlayer->mMaxEnergy)
		{
			data->mPlayer->mCurrentEnergy = data->mPlayer->mMaxEnergy;
		}
	}

	// SHOOT COOLDOWN
	if (data->mPlayer->mOverheatCooldown > ZERO)
	{
		data->mPlayer->mOverheatCooldown -= game->mGameDt;
	}
}

void EndGame(Game* _game, struct GameScreenData* _data)
{
	if (Entity_IsDead(_data->mPlayer))
	{
		if (_game->mScore > _data->mNextBossScore)
		{
			_game->mScore = _data->mNextBossScore;
		}
		PushEndScreen(_game);
	}
}

int RandomInt(int min, int max)
{
	if (min < max)
	{
		return (rand() % (max - min + 1)) + min;
	}
	else
	{
		return min;
	}
}
