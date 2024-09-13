#include "MusicBuffer.h"
#include "memory.h"
#include <logger.h>
#include <AL/alext.h>

namespace ns::audio {

MusicBuffer::MusicBuffer(const char* filename)
{
    alGenSources(1, &source);
    alGenBuffers(NUM_BUFFERS, buffers);
    usize frame_size;
    snd_file = sf_open(filename, SFM_READ, &sf_info);
    if (!snd_file)
        NS_ERROR("Could not open music file : %s", filename);
    if (sf_info.channels==1) {
        format = AL_FORMAT_MONO16;
    } else if (sf_info.channels==2) {
        format = AL_FORMAT_STEREO16;
    } else if (sf_info.channels==3) {
        if (sf_command(snd_file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT2D_16;
    } else if (sf_info.channels==4) {
        if (sf_command(snd_file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT3D_16;
    } if (!format) {
        sf_close(snd_file);
        snd_file=nullptr;
        NS_ERROR("Unsupported channel count from file %s", filename);
    }
    frame_size=((usize)BUFFER_SAMPLES*(usize)sf_info.channels) * sizeof(i16);
    membuff = static_cast<short*>(ns::alloc_raw(frame_size));
}

MusicBuffer::~MusicBuffer()
{
    alDeleteSources(1, &source);
    if (snd_file) {
        sf_close(snd_file);
        snd_file = nullptr;
    }
    ns::free_raw(membuff, ((usize)BUFFER_SAMPLES*(usize)sf_info.channels) * sizeof(i16));
    alDeleteBuffers(NUM_BUFFERS, buffers);
}

void MusicBuffer::play()
{
    ALsizei i;
    alGetError();
    alSourceRewind(source);
    alSourcei(source, AL_BUFFER, 0);
    for (i = 0; i < NUM_BUFFERS; i++)
    {
        sf_count_t slen = sf_readf_short(snd_file, membuff, BUFFER_SAMPLES);
        if (slen < 1) break;
        slen *= sf_info.channels * (sf_count_t)sizeof(short);
        alBufferData(buffers[i], format, membuff, (ALsizei)slen, sf_info.samplerate);
    }
    if (alGetError() != AL_NO_ERROR) NS_ERROR("Error buffering for playback");
    alSourceQueueBuffers(source, i, buffers);
    alSourcePlay(source);
    if (alGetError() != AL_NO_ERROR) NS_ERROR("Error starting playback");
}

void MusicBuffer::pause()
{
    alSourcePause(source);
}

void MusicBuffer::stop()
{
    alSourceStop(source);
}

void MusicBuffer::resume()
{
    alSourcePlay(source);
}

void MusicBuffer::update_buffer_stream()
{
    ALint processed, state;
    alGetError();
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
    if (alGetError() != AL_NO_ERROR) NS_ERROR("error checking music source state");
    while (processed > 0)
    {
        ALuint bufid;
        sf_count_t slen;
        alSourceUnqueueBuffers(source, 1, &bufid);
        processed--;
        slen = sf_readf_short(snd_file, membuff, BUFFER_SAMPLES);
        if (slen > 0)
        {
            slen *= sf_info.channels * (sf_count_t)sizeof(short);
            alBufferData(bufid, format, membuff, (ALsizei)slen, sf_info.samplerate);
            alSourceQueueBuffers(source, 1, &bufid);
        }
        if (alGetError() != AL_NO_ERROR) NS_ERROR("error buffering music data");
        if (state != AL_PLAYING && state != AL_PAUSED)
        {
            ALint queued;
            alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
            if (queued==0) return;
            alSourcePlay(source);
            if (alGetError() != AL_NO_ERROR) NS_ERROR("error restarting music playback");
        }
    }
}

ALint MusicBuffer::get_source() { return source; }
bool MusicBuffer::is_playing()
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

} // namespace ns::audio
