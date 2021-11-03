#include "Obstacle.h"

void InitObstacle(Obstacle** _obstacle)
{
	Obstacle* newObstacle = (Obstacle*)malloc(sizeof(Obstacle));
	memset(newObstacle, 0, sizeof(Obstacle));

	*_obstacle = newObstacle;

	Entity_Initialize(&(*_obstacle)->entity, 1, 1, 5, Obstacle_Update);

	InitDisplayZone // TEMPORARY
	(
		&newObstacle->entity.displayZone,
		WINDOW_WIDTH,
		(rand() % WINDOW_HEIGHT) - 1,
		2, 2, 1
	);

	DrawRectangleInDisplayZone
	(
		&newObstacle->entity.displayZone,
		0, 0, 2, 2,
		WHITE, YELLOW, ' '
	);

	newObstacle->entity.position_x = WINDOW_WIDTH;
	newObstacle->entity.position_y = newObstacle->entity.displayZone.mPosY;
}

void Obstacle_Update(Obstacle* _obstacle, Game* _game)
{
	Obstacle_UpdateMovement(_obstacle, _game);
	FlushDisplayZone(_game->mDisplaySettings, &_obstacle->entity.displayZone);
}

void Obstacle_UpdateMovement(Obstacle* _obstacle, Game* _game)
{
	double newpos_x = 
		_obstacle->entity.position_x -
		(_obstacle->entity.speed * _game->mGameDt * 10);

	if (newpos_x + _obstacle->entity.displayZone.mSizeX + 1 < 0)
	{
		// Free and delete from entity list if out of the screen
		PopEntity(_game, &_obstacle->entity);
	}
	else
	{
		Entity_MoveTo(&_obstacle->entity, newpos_x, _obstacle->entity.position_y);
	}
}
