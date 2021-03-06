#include "Obstacle.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"

void Obstacle_Initialize(Obstacle** _obstacle, GameScreenData * _gameScreen)
{
	Obstacle* newObstacle = (Obstacle*)malloc(sizeof(Obstacle));
	memset(newObstacle, 0, sizeof(Obstacle));

	*_obstacle = newObstacle;

	ParamSection* obstacleSection = GetSection(_gameScreen->mParamsList, OBSTACLE_INIT_SECTION);

	if (obstacleSection)
	{
		ParamInt* obstacleHealth = (ParamInt*)GetParamInSection(obstacleSection, "Health");
		ParamInt* minObstacleSpeed = (ParamInt*)GetParamInSection(obstacleSection, "Speed_min");
		ParamInt* maxObstacleSpeedMax = (ParamInt*)GetParamInSection(obstacleSection, "Speed_max");

		Entity_Initialize((Entity*)newObstacle, TYPE_OBSTACLE,
			WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - newObstacle->mEntity.mDisplayZone.mSizeY),
			obstacleHealth->mValue, ((float)RandomInt(minObstacleSpeed->mValue, maxObstacleSpeedMax->mValue)),
			&_gameScreen->mSprites[TYPE_OBSTACLE],
			Obstacle_Update, Obstacle_OnCollide, Obstacle_Destroy);
	}
}

void Obstacle_Update(Obstacle* _obstacle, Game* _game, GameScreenData* _gameScreen)
{
	Obstacle_UpdateMovement(_obstacle, _game, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &_obstacle->mEntity.mDisplayZone);
}

void Obstacle_UpdateMovement(Obstacle* _obstacle, Game* _game)
{
	double newpos_x = 
		_obstacle->mEntity.mPosition_x -
		(_obstacle->mEntity.mSpeed * _game->mGameDt * 10);

	Entity_MoveTo(&_obstacle->mEntity, newpos_x, _obstacle->mEntity.mPosition_y);

	if (_obstacle->mEntity.mPosition_x < -5.0 || _obstacle->mEntity.mPosition_x > (double)(WINDOW_WIDTH + 5))
	{
		_obstacle->mEntity.mCurrentHealth = 0;
	}
}

void Obstacle_OnCollide(Obstacle* _current, Entity* _entity, Game* _game)
{
	switch (_entity->mEntityType)
	{
	case TYPE_PLAYER:
	case TYPE_PLAYER_PROJECTILE:
	case TYPE_ENEMY_PROJECTILE:
	case TYPE_ENEMY_KAMIKAZE:
		Entity_Kill((Entity*)_current);
		return;
	default:
		return;
	}
}

void Obstacle_Destroy(Entity* _entity)
{
	free(_entity);
}
