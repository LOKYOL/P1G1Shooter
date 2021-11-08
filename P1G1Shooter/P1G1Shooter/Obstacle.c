#include "Obstacle.h"
#include "GameScreen.h"
#include "Engine/DisplayZoneDrawing.h"

void Obstacle_Initialize(Obstacle** _obstacle)
{
	Obstacle* newObstacle = (Obstacle*)malloc(sizeof(Obstacle));
	memset(newObstacle, 0, sizeof(Obstacle));

	*_obstacle = newObstacle;

	Entity_Initialize(&(*_obstacle)->mEntity, 1, 1, (rand() % 5) + 3, Obstacle_Update);

	InitDisplayZone // TEMPORARY
	(
		&newObstacle->mEntity.mDisplayZone,
		WINDOW_WIDTH,
		(rand() % (WINDOW_HEIGHT - 2)) + 1,
		2, 2, 1
	);

	DrawRectangleInDisplayZone
	(
		&newObstacle->mEntity.mDisplayZone,
		0, 0, 2, 2,
		WHITE, YELLOW, ' '
	);

	newObstacle->mEntity.mPosition_x = WINDOW_WIDTH;
	newObstacle->mEntity.mPosition_y = newObstacle->mEntity.mDisplayZone.mPosY;
	newObstacle->mEntity.mEntityType = TYPE_OBSTACLE;
}

void Obstacle_Update(Obstacle* _obstacle, Game* _game, GameScreenData* _gameScreen)
{
	Obstacle_UpdateMovement(_obstacle, _game, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &_obstacle->mEntity.mDisplayZone);
}

void Obstacle_UpdateMovement(Obstacle* _obstacle, Game* _game, GameScreenData* _gameScreen)
{
	double newpos_x = 
		_obstacle->mEntity.mPosition_x -
		(_obstacle->mEntity.mSpeed * _game->mGameDt * 10);

	Entity_MoveTo(&_obstacle->mEntity, newpos_x, _obstacle->mEntity.mPosition_y);
}
