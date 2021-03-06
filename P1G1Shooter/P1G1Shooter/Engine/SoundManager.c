#include "SoundManager.h"

int FindSoundPriority(char* _SoundName, SoundManager* _sound) 
{
    int n = 3;

    for (int i = 0; i < 6; i++) {
        if (strcmp(_SoundName, _sound->mSounds[i].mSoundPath)) {
            n = _sound->mSounds[i].mPriority;
            break;
        }
    }

    return n;
}

void Play_Sound(char* _SoundName, SoundManager* _sound) 
{
    int mBool = 0;

    if (_sound->mSound == NULL) 
    {
        Sound* newSound = (Sound*)malloc(sizeof(Sound));
        newSound->mSoundPath = _SoundName;
        newSound->mPriority = FindSoundPriority(_SoundName, _sound);
        _sound->mSound = newSound;
        mBool = 1;
    } 
    else if (_sound->mSound != NULL && _sound->mSound->mPriority < FindSoundPriority(_SoundName, _sound)) 
    {
        _sound->mSound->mPriority = FindSoundPriority(_SoundName, _sound);
        _sound->mSound->mSoundPath = _SoundName;
        mBool = 1;
    }

    if (mBool == 1) 
    {
        char soundSearch[50] = "Sounds/";

        snprintf(soundSearch, 50, "Sounds/%s", _sound->mSound->mSoundPath);

        wchar_t wc[50];
        size_t size = strlen(soundSearch) + 1;
        size_t outSize;
        mbstowcs_s(&outSize, wc, size, soundSearch, size - 1);

        PlaySound(wc, NULL, SND_ASYNC);
        free(_sound->mSound);
        _sound->mSound = NULL;
    }
}

void Play_SoundThread(char* _SoundName) 
{
    char soundSearch[50] = "Sounds/";

    snprintf(soundSearch, 50, "Sounds/%s", _SoundName);
    wchar_t wc[50];
    size_t size = strlen(soundSearch) + 1;
    size_t outSize;
    mbstowcs_s(&outSize, wc, size, soundSearch, size - 1);

    PlaySound(wc, NULL, SND_ASYNC | SND_NOSTOP);
}

void Play_Music(char* _MusicName) 
{
    CreateThread(0, 0, Play_MusicThread, _MusicName, 0, NULL);
}

void Play_MusicThread(char* _MusicName) 
{
    char musicSearch[50] = "Music/";

    snprintf(musicSearch, 50, "Music/%s", _MusicName);

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