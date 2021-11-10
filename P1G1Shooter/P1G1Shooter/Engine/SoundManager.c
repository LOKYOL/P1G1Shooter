#include "SoundManager.h"

void Play_SoundThread(char* SoundName) 
{
	char soundSearch[50] = "Sounds/";

	for (int i = 7; i < 50; i++) 
	{
		soundSearch[i] = SoundName[i - 7];
	}

	wchar_t wc[50];
	size_t size = strlen(soundSearch) + 1;
	size_t outSize;
	mbstowcs_s(&outSize, wc, size, soundSearch, size-1);
	
	PlaySound(wc, NULL, SND_ASYNC | SND_FILENAME);
}

void Play_Sound(char* SoundName) 
{
	CreateThread(0, 0, Play_SoundThread, SoundName, 0, NULL);
}

int FindSoundPriority(char* SoundName, SoundManager* _sound) {
    int n = 3;

    for (int i = 0; i < 6; i++) {
        if (strcmp(SoundName, _sound->mSounds[i].mSoundPath)) {
            n = _sound->mSounds[i].mPriority;
            break;
        }
    }

    return n;
}

void Play_Sound(char* SoundName, SoundManager* _sound) 
{
    int mBool = 0;

    if (_sound->mSound == NULL) 
    {
        Sound* newSound;
        newSound->mSoundPath = SoundName;
        newSound->mPriority = FindSoundPriority(SoundName, _sound);
        _sound->mSound = newSound;
        mBool = 1;
    } else if (_sound->mSound != NULL && _sound->mSound->mPriority >= FindSoundPriority(SoundName, _sound)) {
        _sound->mSound->mPriority = FindSoundPriority(SoundName, _sound);
        _sound->mSound->mSoundPath = SoundName;
        mBool = 1;
    }

    if (mBool == 1) {
        char soundSearch[50] = "Sounds/";

        snprintf(soundSearch, 50, "Sounds/%s", SoundName);

        wchar_t wc[50];
        size_t size = strlen(soundSearch) + 1;
        size_t outSize;
        mbstowcs_s(&outSize, wc, size, soundSearch, size - 1);

        PlaySound(wc, NULL, SND_SYNC);
        free(_sound->mSound);
    }
}

void Play_SoundThread(char* SoundName) 
{
    char soundSearch[50] = "Sounds/";

    snprintf(soundSearch, 50, "Sounds/%s", SoundName);

    wchar_t wc[50];
    size_t size = strlen(soundSearch) + 1;
    size_t outSize;
    mbstowcs_s(&outSize, wc, size, soundSearch, size - 1);

    PlaySound(wc, NULL, SND_ASYNC | SND_NOSTOP);
}

void Play_Music(char* MusicName) 
{
    CreateThread(0, 0, Play_MusicThread, MusicName, 0, NULL);
}

void Play_MusicThread(char* MusicName) 
{
    char musicSearch[50] = "Music/";

    snprintf(musicSearch, 50, "Music/%s", MusicName);

    wchar_t wcMusic[50];
    size_t size = strlen(musicSearch) + 1;
    size_t outSize;
    mbstowcs_s(&outSize, wcMusic, size, musicSearch, size - 1);

    PlaySound(wcMusic, NULL, SND_ASYNC | SND_NOSTOP);
}

void InitSoundManager(SoundManager* _soundManager)
{
    _soundManager->mSounds[0].mPriority = 2;
    _soundManager->mSounds[0].mSoundPath = "player_shoot.wav";

    _soundManager->mSounds[1].mPriority = 1;
    _soundManager->mSounds[1].mSoundPath = "player_recharge.wav";

    _soundManager->mSounds[2].mPriority = 1;
    _soundManager->mSounds[2].mSoundPath = "player_enemyhit.wav";

    _soundManager->mSounds[3].mPriority = 0;
    _soundManager->mSounds[3].mSoundPath = "player_die.wav";

    _soundManager->mSounds[4].mPriority = 2;
    _soundManager->mSounds[4].mSoundPath = "enemy_hit.wav";

    _soundManager->mSounds[5].mPriority = 1;
    _soundManager->mSounds[5].mSoundPath = "enemy_die.wav";
}