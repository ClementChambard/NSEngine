#include "SoundSource.h"
#include <AL/al.h>

namespace ns::audio {

Source::Source()
{
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, pitch);
    alSourcef(source, AL_GAIN, gain);
    alSource3f(source, AL_POSITION, position[0], position[1], position[2]);
    alSource3f(source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
    alSourcei(source, AL_LOOPING, loop_sound);
    alSourcei(source, AL_BUFFER, buffer);
}

Source::~Source()
{
    alDeleteSources(1, &source);
}

void Source::play(u32 buffer_to_play)
{
    if (buffer_to_play != buffer)
    {
        buffer = buffer_to_play;
        alSourcei(source, AL_BUFFER, (ALint) buffer);
    }

    alSourcePlay(source);
}

void Source::stop()
{
    alSourceStop(source);
}

void Source::pause()
{
    alSourcePause(source);
}

void Source::resume()
{
    alSourcePlay(source);
}

void Source::set_buffer_to_play(u32 buffer_to_play)
{
    if (buffer_to_play != buffer)
    {
        buffer = buffer_to_play;
        alSourcei(source, AL_BUFFER, (ALint) buffer);
    }
}

void Source::set_looping(bool loop)
{
    alSourcei(source, AL_LOOPING, (ALint) loop);
}

bool Source::is_playing()
{
    ALint play_state;
    alGetSourcei(source, AL_SOURCE_STATE, &play_state);
    return (play_state == AL_PLAYING);
}

}
