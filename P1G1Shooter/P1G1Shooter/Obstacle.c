#include "Obstacle.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"

void Obstacle_Initialize(Obstacle** _obstacle, GameScreenData* gameScreen)
{
	Obstacle* newObstacle = (Obstacle*)malloc(sizeof(Obstacle));
	memset(newObstacle, 0, sizeof(Obstacle));

	*_obstacle = newObstacle;

	Entity_Initialize(newObstacle, TYPE_OBSTACLE, 
		WINDOW_WIDTH, rand() % (WINDOW_HEIGHT - newObstacle->mEntity.mDisplayZone.mSizeY),
		OBSTACLE_HEALTH, rand() % 5 + 3, &gameScreen->mSprites[TYPE_OBSTACLE],
		Obstacle_Update, Obstacle_OnCollide, Obstacle_Destroy);
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

	if (_obstacle->mEntity.mPosition_x < -5.0 || _obstacle->mEntity.mPosition_x > WINDOW_WIDTH + 5)
	{
		_obstacle->mEntity.mHealth = 0;
	}
}

void Obstacle_OnCollide(Entity* _entity, Game* game)
{
	EntityType type = _entity->mEntityType;
	if (type == TYPE_PLAYER || type == TYPE_ENEMY_KAMIKAZE)
	{
		Entity_TakeDamages(_entity, OBSTACLE_DAMAGES);

		if (type == TYPE_PLAYER)
		{
			Play_Sound("player_enemyhit", game->mSoundManager);
		}
	}
	else if (_entity->mEntityType == TYPE_PLAYER_PROJECTILE)
	{
		Entity_Kill(_entity);
	}
}

void Obstacle_Destroy(Entity* _entity)
{
	free(_entity);
}
