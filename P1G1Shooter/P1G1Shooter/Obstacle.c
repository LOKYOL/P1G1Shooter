#include "Obstacle.h"
#include "Engine/Game.h"
#include "Engine/DisplayZoneDrawing.h"

void InitObstacle(Obstacle** _obstacle)
{
	Obstacle* newObstacle = (Obstacle*)malloc(sizeof(Obstacle));
	memset(newObstacle, 0, sizeof(Obstacle));

	*_obstacle = newObstacle;

	Entity_Initialize(&(*_obstacle)->entity, 1, 1, 5, Obstacle_Update);

	InitDisplayZone // TEMPORARY
	(
		&newObstacle->entity.mDisplayZone,
		WINDOW_WIDTH,
		(rand() % WINDOW_HEIGHT) - 1,
		2, 2, 1
	);

	DrawRectangleInDisplayZone
	(
		&newObstacle->entity.mDisplayZone,
		0, 0, 2, 2,
		WHITE, YELLOW, ' '
	);

	newObstacle->entity.mPosition_x = WINDOW_WIDTH;
	newObstacle->entity.mPosition_y = newObstacle->entity.mDisplayZone.mPosY;
	newObstacle->entity.mEntityType = TYPE_OBSTACLE;
}

void Obstacle_Update(Obstacle* _obstacle, Game* _game)
{
	Obstacle_UpdateMovement(_obstacle, _game);
	FlushDisplayZone(_game->mDisplaySettings, &_obstacle->entity.mDisplayZone);
}

void Obstacle_UpdateMovement(Obstacle* _obstacle, Game* _game)
{
	double newpos_x = 
		_obstacle->entity.mPosition_x -
		(_obstacle->entity.mSpeed * _game->mGameDt * 10);

	if (newpos_x + _obstacle->entity.mDisplayZone.mSizeX + 1 < 0)
	{
		// Free and delete from entity list if out of the screen
		PopEntity(_game, &_obstacle->entity);
		Entity_Free(_obstacle);
	}
	else
	{
		Entity_MoveTo(&_obstacle->entity, newpos_x, _obstacle->entity.mPosition_y);
	}
}
