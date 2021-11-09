#include "SoundManager.h"

void Play_SoundThread(char* SoundName) {
	char soundSearch[18] = "Sounds/";

	for (int i = 7; i < 18; i++) {
		soundSearch[i] = SoundName[i - 7];
	}

	wchar_t wc[18];
	size_t size = strlen(soundSearch) + 1;
	size_t outSize;
	mbstowcs_s(&outSize, wc, size, soundSearch, size-1);
	
	PlaySound(wc, NULL, SND_ASYNC | SND_FILENAME);
}

void Play_Sound(char* SoundName) {
	DWORD myThreadID;
	CreateThread(0, 0, Play_SoundThread, SoundName, 0, &myThreadID);
}