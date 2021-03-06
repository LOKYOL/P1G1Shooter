#include "BossScreen.h"
#include "Engine/DVector.h"
#include "Player.h"
#include "Boss.h"

int BossScreenInit(Game* _game, GameState* _gameState)
{
	_gameState->mData = malloc(sizeof(BossScreenData));
	BossScreenData* data = _gameState->mData;

	data->mGameScreenData = ((GameState*)DVectorGet(_game->mStateStack, _game->mStateStack->mCurrentSize - 2))->mData;

	Boss_Initialize(&data->mBoss, data->mGameScreenData);

	return 0;
}

int BossScreenUpdate(Game* _game, GameState* _gameState)
{
	BossScreenData* data = _gameState->mData;
	GameScreenData* gameData = data->mGameScreenData;

	ClearBuffer(_game->mDisplaySettings, gameData->mFG, gameData->mBG, 178);

	// Update Boss
	data->mBoss->mEntity.mUpdate(data->mBoss, _game, gameData);
	UpdateEntity(_game, gameData);

	UpdateWeapon(_game, gameData);

	// COLLISIONS
	HandleCollision(gameData->mAllEntities, _game);
	HandleEntityCollision(
		(Entity*)gameData->mPlayer,
		gameData->mAllEntities->mBuffer,
		gameData->mAllEntities->mCurrentSize,
		_game);
	HandleEntityCollision(
		(Entity*)data->mBoss,
		gameData->mAllEntities->mBuffer,
		gameData->mAllEntities->mCurrentSize,
		_game);

	HandleEntitiesCollision((Entity*)data->mBoss, gameData->mPlayer, _game);


	EndGame(_game, gameData);
	EndBoss(data->mBoss, _game);
	return 0;
}

int BossScreenClose(Game* _game, GameState* _gameState)
{
	BossScreenData* data = _gameState->mData;

	_game->mScore = data->mGameScreenData->mNextBossScore;

	data->mGameScreenData->mNextBossScore += data->mGameScreenData->mNextBossScore;
	data->mGameScreenData->mGameSpawnEnemyKamikazeTimer = 1;
	data->mGameScreenData->mGameSpawnEnemyTimer = 0;
	data->mGameScreenData->mGameSpawnObstacleTimer = 0;

	data->mBoss->mEntity.mDestroy(data->mBoss);
	data->mBoss = NULL;

	free(data->mBoss);
	free(data->mGameScreenData);
	free(data);

	return 0;
}

void EndBoss(Boss* _boss, Game* _game)
{
	if (_boss->mEntity.mCurrentHealth <= 0)
	{
		PopGameState(_game);
	}
}
