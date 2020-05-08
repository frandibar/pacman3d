#include "soundplayer.h"

//constructor
SoundPlayer::SoundPlayer(const string &wavfile) 
{ 
    //inicializacion de audio
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;
    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
        fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
    }
    //crea sonido
    _sound = Mix_LoadWAV(wavfile.c_str());
    if (!_sound) {
        fprintf(stderr, "Unable to load WAV file: %s\n", Mix_GetError());
    }
}

SoundPlayer::~SoundPlayer()
{
    if (_sound)
        Mix_FreeChunk(_sound);
    Mix_CloseAudio();
}

void SoundPlayer::play(bool wait) 
{
    if (_sound) {
        int channel;
        channel = Mix_PlayChannel(-1, _sound, 0);
        if (channel == -1)
            return;
        if (wait)
            while (Mix_Playing(channel) != 0)
                ;
    }
}


