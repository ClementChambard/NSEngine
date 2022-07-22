#include "SoundBuffer.h"
#include "Error.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL/alext.h>
namespace NSEngine {

    std::vector<ALuint> SoundBuffer::SoundEffectBuffers;

    ALuint SoundBuffer::addSoundEffect(const char* filename)
    {
        ALenum err, format;
        ALuint buffer;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* membuff;
        sf_count_t num_frames;
        ALsizei num_bytes;

        sndfile = sf_open(filename, SFM_READ, &sfinfo);
        if (!sndfile) { error("Could not open audio in", filename, ":", sf_strerror(sndfile)); return 0; }
        if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short)) / sfinfo.channels)
        { error("Bad sample count in", filename, ":", sfinfo.frames); sf_close(sndfile); return 0; }
        format = AL_NONE;
        if (sfinfo.channels == 1) format = AL_FORMAT_MONO16;
        else if (sfinfo.channels == 2) format = AL_FORMAT_STEREO16;
        else if (sfinfo.channels == 3)
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT2D_16;
        if (!format)
        { error("Unsupperted channel count :", sfinfo.channels); sf_close(sndfile); return 0; }

        membuff = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
        num_frames = sf_readf_short(sndfile, membuff, sfinfo.frames);
        if (num_frames < 1)
        { free(membuff); sf_close(sndfile); error("Failed to read samples in", filename, ":", num_frames); return 0; }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuff, num_bytes, sfinfo.samplerate);
        free(membuff);
        sf_close(sndfile);

        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            error("OpenAL Error :", alGetString(err));
            if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
            return 0;
        }
        SoundEffectBuffers.push_back(buffer);
        return buffer;

    }

    bool SoundBuffer::removeSoundEffect(const ALuint& buffer)
    {
        auto it = SoundEffectBuffers.begin();
        while (it != SoundEffectBuffers.end())
        {
            if (*it == buffer)
            {
                alDeleteBuffers(1, &*it);

                it = SoundEffectBuffers.erase(it);

                return true;
            }
            else ++it;
        }
        return false;
    }

    void SoundBuffer::Init() 
    {
        SoundEffectBuffers.clear();
    }

    void SoundBuffer::Delete() 
    {
        alDeleteBuffers(SoundEffectBuffers.size(), SoundEffectBuffers.data());

        SoundEffectBuffers.clear();
    }

}
