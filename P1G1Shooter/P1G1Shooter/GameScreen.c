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


int GameScreenInit(Game* _game, GameState* _state)
{
	_state->mData = malloc(sizeof(GameScreenData));
	GameScreenData* data = _state->mData;
	
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

	_game->mScore = ZERO;
	data->mScoreDisplayZone = malloc(sizeof(DisplayZone));
	InitDisplayZone(data->mScoreDisplayZone, ZERO, ZERO, 20, 1, ZERO);

	return 0;
}

void LoadSpritesFromIni(GameScreenData* _data)
{
	ParamSection* spritesSection = GetSection(_data->mParamsList, "Sprites");

	if (spritesSection) {
		ParamInt* spritesSize = (ParamInt*)GetParamInSection(spritesSection, "SIZEOF_SPRITES_NAMES");

		if (spritesSize) {
			_data->mSprites = (DisplayZone*)malloc(sizeof(DisplayZone) * spritesSize->mValue);

			DisplayZone* curDisplayZone = NULL;
			char* path = (char*)malloc(sizeof(char) * 200);
			char* spriteParam = (char*)malloc(sizeof(char) * 10);

			for (int i = 0; i < spritesSize->mValue; i++)
			{
				snprintf(spriteParam, 10, "Sprite_%d", i);
				GetParamElemString(spritesSection, path, 200, spriteParam);
				curDisplayZone = CreateDisplayZoneFromBMP(path);
				_data->mSprites[i] = *curDisplayZone;
				free(curDisplayZone); 
			}

			free(spriteParam);
			free(path);
		}
	}
}

int GameScreenClose(Game* _game, GameState* _state)
{
	GameScreenData* data = _state->mData;

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

	free(_state->mData);

	return 0;
}

int GameScreenUpdate(Game* _game, GameState* _state)
{
	GameScreenData* data = _state->mData;

	SpawnEntity(_game, data);

	UpdateEntity(_game, data);

	UpdateWeapon(_game, data);

	// COLLISIONS
	HandleCollision(data->mAllEntities,	_game);
	HandleEntityCollision((Entity*)data->mPlayer, data->mAllEntities->mBuffer, data->mAllEntities->mCurrentSize, _game);

	if (_game->mScore >= data->mNextBossScore)
	{
		PushBossScreen(_game);
	}

	char totalScore[20] = "";
	snprintf(totalScore, 19, "Score : %d", _game->mScore);
	PrintInDisplayZone(data->mScoreDisplayZone, WHITE, BLACK, ZERO, ZERO, totalScore, ZERO, NO_FLAG);

	EndGame(_game, data);
	
	FlushDisplayZone(_game->mDisplaySettings, data->mScoreDisplayZone);

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

void HandleCollision(DVector* _list, Game* _gameStruct)
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
				_gameStruct
			);
		}
	}
}

void HandleEntityCollision(Entity* _entity, Entity** _list, int _length, Game* _gameStruct)
{
	Entity* curCompare = NULL;
	for (int i = ZERO; i < _length; i++)
	{
		HandleEntitiesCollision(_entity, _list[i], _gameStruct);
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

		// SCORE

		if (_entityA->mCurrentHealth == ZERO && _entityA->mEntityType == TYPE_PLAYER_PROJECTILE)
		{
			if (_entityB->mEntityType == TYPE_ENEMY_SHOOTER)
			{
				//score += 3
				_game->mScore += 3;
			}
			else if (_entityB->mEntityType == TYPE_ENEMY_KAMIKAZE)
			{
				//score += 4
				_game->mScore += 4;
			}
		}

		if (_entityB->mCurrentHealth == ZERO && _entityB->mEntityType == TYPE_PLAYER_PROJECTILE)
		{
			if (_entityA->mEntityType == TYPE_ENEMY_SHOOTER)
			{
				_game->mScore += 3;
				//score += 3
			}
			else if (_entityA->mEntityType == TYPE_ENEMY_KAMIKAZE)
			{
				_game->mScore += 4;
				//score += 4
			}
			else if (_entityA->mEntityType == TYPE_OBSTACLE)
			{
				_game->mScore += 1;
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

char InRange(int _value, int _min, int _max)
{
	return _value < _max && _value > _min;
}

void SpawnEntity(Game* _game, GameScreenData* _data)
{
	// SPAWN OBSTACLE
	_data->mGameSpawnObstacleTimer += _game->mGameDt;
	if (_data->mGameSpawnObstacleTimer >= OBSTACLE_SPAWN_TIMER)
	{
		_data->mGameSpawnObstacleTimer -= OBSTACLE_SPAWN_TIMER;

		SpawnObstacle(_data);
	}

	// SPAWN ENEMY
	_data->mGameSpawnEnemyTimer += _game->mGameDt;
	_data->mGameSpawnEnemyKamikazeTimer += _game->mGameDt;

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

void SpawnHealthPowerup(GameScreenData* _game, double _posX, double _posY)
{
	PowerupHealth* newPowerup = NULL;
	PowerupHealth_Initialize(&newPowerup, _game, _posX, _posY);
	DVectorPushBack(_game->mAllEntities, &newPowerup);
}

void SpawnAimAssistPowerup(GameScreenData* _game, double _posX, double _posY)
{
	PowerupAimAssist* newPowerup = NULL;
	PowerupAimAssist_Initialize(&newPowerup, _game, _posX, _posY);
	DVectorPushBack(_game->mAllEntities, &newPowerup);
}

char PopBackIfIsDead(GameScreenData* _game, Entity* _entity, Game* _gameStruct)
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

void UpdateEntity(Game* _game, GameScreenData* _data)
{
	_data->mPlayer->mEntity.mUpdate(_data->mPlayer, _game, _data);

	// FOR EACH ENTITY
	Entity* curEntity = NULL;
	for (unsigned int i = 0; i < _data->mAllEntities->mCurrentSize; i++)
	{
		curEntity = DVectorGetTyped(_data->mAllEntities, Entity*, i);

		if (curEntity->mUpdate != NULL)
		{
			curEntity->mUpdate((void*)curEntity, _game, _data);
		}

		if (PopBackIfIsDead(_data, DVectorGetTyped(_data->mAllEntities, Entity*, i), _game))
		{
			i--;
		}
	}
}

void UpdateWeapon(Game* _game, GameScreenData* _data)
{
	// ENERGY RECHARGE
	if (_data->mPlayer->mCurrentReloadCooldown > ZERO)
	{
		_data->mPlayer->mCurrentReloadCooldown -= _game->mGameDt;
	}
	else
	{
		_data->mPlayer->mCurrentEnergy += (float)(_game->mGameDt * _data->mPlayer->mReloadGain);

		if (_data->mPlayer->mCurrentEnergy >= _data->mPlayer->mMaxEnergy)
		{
			_data->mPlayer->mCurrentEnergy = _data->mPlayer->mMaxEnergy;
		}
	}

	// SHOOT COOLDOWN
	if (_data->mPlayer->mOverheatCooldown > ZERO)
	{
		_data->mPlayer->mOverheatCooldown -= _game->mGameDt;
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

int RandomInt(int _min, int _max)
{
	if (_min < _max)
	{
		return (rand() % (_max - _min + 1)) + _min;
	}
	else
	{
		return _min;
	}
}
