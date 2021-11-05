#include "projectile.h"
#include "Engine/Game.h"

void InitProj(Projectile** proj, int speed, int direction, double posPlayer_x, double posPlayer_y)
{
	(*proj) = (Projectile*)malloc(sizeof(Projectile));
	InitDisplayZone(&(*proj)->mEntity.mDisplayZone, (posPlayer_x + 3), posPlayer_y, 2, 1, 1);

	DrawRectangleInDisplayZone
	(
		&(*proj)->mEntity.mDisplayZone,
		0, 0, 2, 1,
		WHITE, RED, ' '
	);

	Entity_Initialize(&(*proj)->mEntity, 1, 1, WINDOW_WIDTH / 4, Projectile_Update);
	(*proj)->mEntity.mPosition_x = (posPlayer_x + 3);
	(*proj)->mEntity.mPosition_y = posPlayer_y;
	(*proj)->direction = 1;
	(*proj)->mEntity.mEntityType = TYPE_PLAYER_PROJECTILE;
}

void Projectile_Update(void* _proj, Game* _game, GameScreenData* _gameScreen) {

	Projectile* myProjectile = (Projectile*)_proj;

	Projectile_UpdateMovement(myProjectile, _game, _gameScreen);
	FlushDisplayZone(_game->mDisplaySettings, &myProjectile->mEntity.mDisplayZone);
}

void Projectile_UpdateMovement(Projectile * proj, Game* _game, GameScreenData* _gameScreen)
{
	double newpos_x = proj->mEntity.mPosition_x, newpos_y = proj->mEntity.mPosition_y;

		if (proj->direction)
		{
			newpos_x += proj->mEntity.mSpeed * _game->mGameDt;
		}
		else
		{
			newpos_y -= proj->mEntity.mSpeed * _game->mGameDt;
		}

		Entity_MoveTo(&proj->mEntity, newpos_x, newpos_y);
}

//int InRange(int value, int min, int max)
//{
//	return (value >= min && value <= max) ? 1 : 0;
//}