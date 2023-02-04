#include "SoundSource.h"
#include "Error.h"
namespace NSEngine {

    SoundSource::SoundSource() 
    {
        alGenSources(1, &Source);
        alSourcef(Source, AL_PITCH, Pitch);
        alSourcef(Source, AL_GAIN, Gain);
        alSource3f(Source, AL_POSITION, Position[0], Position[1], Position[2]);
        alSource3f(Source, AL_VELOCITY, Velocity[0], Velocity[1], Velocity[2]);
        alSourcei(Source, AL_LOOPING, LoopSound);
        alSourcei(Source, AL_BUFFER, Buffer);
    }

    SoundSource::~SoundSource() 
    {
        alDeleteSources(1, &Source);
    }

    void SoundSource::Play(const ALuint& buffer_to_play)
    {
        if (buffer_to_play != Buffer)
        {
            Buffer = buffer_to_play;
            alSourcei(Source, AL_BUFFER, (ALint)Buffer);
        }

        alSourcePlay(Source);
    }

    void SoundSource::Stop()
    {
        alSourceStop(Source);
    }

    void SoundSource::Pause()
    {
        alSourcePause(Source);
    }

    void SoundSource::Resume()
    {
        alSourcePlay(Source);
    }

    void SoundSource::SetBufferToPlay(const ALuint& buffer_to_play)
    {
        if (buffer_to_play != Buffer)
        {
            Buffer = buffer_to_play;
            alSourcei(Source, AL_BUFFER, (ALint) Buffer);
        }
    }

    void SoundSource::SetLooping(const bool& loop)
    {
        alSourcei(Source, AL_LOOPING, (ALint) loop);
    }

    bool SoundSource::isPlaying()
    {
        ALint playState;
        alGetSourcei(Source, AL_SOURCE_STATE, &playState);
        return (playState == AL_PLAYING);
    }
}
