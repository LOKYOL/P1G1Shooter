#include "TitleScreen.h"
#include "Game.h"

int TitleScreenInit(struct Game* game, struct GameState* state)
{
	state->mData = malloc(sizeof(TitleScreenData));

	TitleScreenData* datascreen = (TitleScreenData*)state->mData;

	InitDisplayZone(&datascreen->mZoneTitre, 30, 20, 100, 10, 0);
	PrintInDisplayZone(&datascreen->mZoneTitre, RED, BLUE, 5, 3, "Titre de mon jeu", 0, NO_FLAG);

	return 0;

}
int TitleScreenClose(struct Game* game, struct GameState* state)
{
	TitleScreenData* datascreen = (TitleScreenData*)state->mData;
	CloseDisplayZone(&datascreen->mZoneTitre);

	free(state->mData);

	ClearBuffer(game->mDisplaySettings, BLACK, BLACK);

	return 0;
}
int TitleScreenUpdate(struct Game* game, struct GameState* state)
{
	TitleScreenData* datascreen = (TitleScreenData*)state->mData;

	FlushDisplayZone(game->mDisplaySettings, &datascreen->mZoneTitre);

	return 0;
}

