#pragma once
#include <Windows.h>
#include <wchar.h>
#include <stdio.h>

#pragma comment(lib,"winmm.lib")

typedef struct Sound 
{
	char* mSoundPath;
	unsigned char mPriority;
} Sound;

typedef struct SoundManager 
{
	Sound* mSound;
	Sound mSounds[6];
} SoundManager;

int FindSoundPriority(char* SoundName, SoundManager* sound);
void Play_Sound(char* SoundName, SoundManager* sound);
void Play_Music(char* MusicName);
void Play_MusicThread(char* MusicName);
void Play_SoundThread(char* SoundName);
void InitSoundManager(SoundManager* soundManager);