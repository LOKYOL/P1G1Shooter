#pragma once
#include <Windows.h>
#include <wchar.h>
#include <stdio.h>

typedef struct Sound {
	char* mSoundPath;
	unsigned char mPriority;
} Sound;

typedef struct SoundManager {
	Sound* mSound;
	Sound mSounds[6];
} SoundManager;

void Play_Sound(char* SoundName);
void Play_Music(char* MusicName);
void Play_MusicThread(char* MusicName);
void Play_SoundThread(char* SoundName);
void InitSoundManager(SoundManager* _soundManager);