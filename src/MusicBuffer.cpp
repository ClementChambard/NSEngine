#include "MusicBuffer.h"
#include "Error.h"
#include <AL/alext.h>

namespace NSEngine {

    MusicBuffer::MusicBuffer(const char* filename) 
    {
        alGenSources(1, &Source);
        alGenBuffers(NUM_BUFFERS, Buffers);
        std::size_t frame_size;
        SndFile = sf_open(filename, SFM_READ, &SfInfo);
        if (!SndFile) error("Could not open music file :", filename);
        if (SfInfo.channels==1) Format = AL_FORMAT_MONO16;
        else if (SfInfo.channels==2) Format = AL_FORMAT_STEREO16;
        else if (SfInfo.channels==3) {
            if (sf_command(SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0)==SF_AMBISONIC_B_FORMAT) Format = AL_FORMAT_BFORMAT2D_16; }
        else if (SfInfo.channels==4) {
            if (sf_command(SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0)==SF_AMBISONIC_B_FORMAT) Format = AL_FORMAT_BFORMAT3D_16; }
        if (!Format) { sf_close(SndFile); SndFile=nullptr; error("Unsupported channel count from file", filename);}
        frame_size=((size_t)BUFFER_SAMPLES*(size_t)SfInfo.channels) * sizeof(short);
        Membuff = static_cast<short*>(malloc(frame_size));
    }

    MusicBuffer::~MusicBuffer() 
    {
        alDeleteSources(1, &Source);
        if (SndFile) sf_close(SndFile); SndFile = nullptr;
        free(Membuff);
        alDeleteBuffers(NUM_BUFFERS, Buffers);
    }
    
    void MusicBuffer::Play()
    {
        ALsizei i;
        alGetError();
        alSourceRewind(Source);
        alSourcei(Source, AL_BUFFER, 0);
        for (i = 0; i < NUM_BUFFERS; i++)
        {
            sf_count_t slen = sf_readf_short(SndFile, Membuff, BUFFER_SAMPLES);
            if (slen < 1) break;
            slen *= SfInfo.channels * (sf_count_t)sizeof(short);
            alBufferData(Buffers[i], Format, Membuff, (ALsizei)slen, SfInfo.samplerate);
        }
        if (alGetError() != AL_NO_ERROR) error("Error buffering for playback");
        alSourceQueueBuffers(Source, i, Buffers);
        alSourcePlay(Source);
        if (alGetError() != AL_NO_ERROR) error("Error starting playback");
    }

    void MusicBuffer::Pause()
    {
        alSourcePause(Source);
    }

    void MusicBuffer::Stop()
    {
        alSourceStop(Source);
    }

    void MusicBuffer::Resume()
    {
        alSourcePlay(Source);
    }

    void MusicBuffer::UpdateBufferStream()
    {
        ALint processed, state;
        alGetError();
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        alGetSourcei(Source, AL_BUFFERS_PROCESSED, &processed);
        if (alGetError() != AL_NO_ERROR) error("error checking music source state");
        while (processed > 0)
        {
            ALuint bufid;
            sf_count_t slen;
            alSourceUnqueueBuffers(Source, 1, &bufid);
            processed--;
            slen = sf_readf_short(SndFile, Membuff, BUFFER_SAMPLES);
            if (slen > 0)
            {
                slen *= SfInfo.channels * (sf_count_t)sizeof(short);
                alBufferData(bufid, Format, Membuff, (ALsizei)slen, SfInfo.samplerate);
                alSourceQueueBuffers(Source, 1, &bufid);
            }
            if (alGetError() != AL_NO_ERROR) error ("error buffering music data");
            if (state != AL_PLAYING && state != AL_PAUSED)
            {
                ALint queued;
                alGetSourcei(Source, AL_BUFFERS_QUEUED, &queued);
                if (queued==0) return;
                alSourcePlay(Source);
                if (alGetError() != AL_NO_ERROR) error("error restarting music playback");
            }
        }
    }

    ALint MusicBuffer::getSource() { return Source; }
    bool MusicBuffer::isPlaying() 
    {
        ALint state;
        alGetSourcei(Source, AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }
}
