#include "SoundBuffer.h"
#include "files/wav.h"
#include <logger.h>
#include <memory.h>
#include <sndfile.h>
#include <AL/alext.h>

namespace ns::audio::sound_buffer {

std::vector<ALuint> sound_effect_buffers;

ALuint add_sound_effect(const char* filename)
{
    WAV_File file(filename);

    ALenum format = AL_NONE;
    if (file.channel_cnt == 1) {
        format = AL_FORMAT_MONO16;
    } else if (file.channel_cnt == 2) {
        format = AL_FORMAT_STEREO16;
    }

    if (!format) {
        NS_ERROR("Unsupperted channel count : %d", file.channel_cnt);
        return 0;
    }

    ALuint buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, file.data, file.data_length, file.sample_rate);

    ALenum err = alGetError();
    if (err != AL_NO_ERROR)
    {
        NS_ERROR("OpenAL Error : %s", alGetString(err));
        if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
        return 0;
    }
    sound_effect_buffers.push_back(buffer);
    return buffer;

}

// ALuint add_sound_effect_libsndfile(const char* filename)
// {
//     ALenum err, format;
//     ALuint buffer;
//     SNDFILE* sndfile;
//     SF_INFO sfinfo;
//     short* membuff;
//     sf_count_t num_frames;
//     ALsizei num_bytes;
//
//     sndfile = sf_open(filename, SFM_READ, &sfinfo);
//     if (!sndfile) {
//         NS_ERROR("Could not open audio in %s: %s", filename, sf_strerror(sndfile));
//         return 0;
//     }
//     if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short)) / sfinfo.channels) {
//         NS_ERROR("Bad sample count in %s: %s", filename, sfinfo.frames);
//         sf_close(sndfile);
//         return 0;
//     }
//     format = AL_NONE;
//     if (sfinfo.channels == 1) {
//         format = AL_FORMAT_MONO16;
//     } else if (sfinfo.channels == 2) {
//         format = AL_FORMAT_STEREO16;
//     } else if (sfinfo.channels == 3) {
//         if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) format = AL_FORMAT_BFORMAT2D_16;
//     }
//     if (!format) {
//         NS_ERROR("Unsupperted channel count : %d", sfinfo.channels);
//         sf_close(sndfile);
//         return 0;
//     }
//
//     membuff = static_cast<short*>(ns::alloc_raw((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
//     num_frames = sf_readf_short(sndfile, membuff, sfinfo.frames);
//     if (num_frames < 1) {
//         ns::free_raw(membuff, (size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short));
//         sf_close(sndfile);
//         NS_ERROR("Failed to read samples in %s: %d", filename, num_frames);
//         return 0;
//     }
//     num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
//
//     buffer = 0;
//     alGenBuffers(1, &buffer);
//     alBufferData(buffer, format, membuff, num_bytes, sfinfo.samplerate);
//     ns::free_raw(membuff, (size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short));
//     sf_close(sndfile);
//
//     err = alGetError();
//     if (err != AL_NO_ERROR)
//     {
//         NS_ERROR("OpenAL Error : %s", alGetString(err));
//         if (buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
//         return 0;
//     }
//     sound_effect_buffers.push_back(buffer);
//     return buffer;
//
// }

bool remove_sound_effect(const u32& buffer)
{
    auto it = sound_effect_buffers.begin();
    while (it != sound_effect_buffers.end())
    {
        if (*it == buffer)
        {
            alDeleteBuffers(1, &*it);

            it = sound_effect_buffers.erase(it);

            return true;
        }
        else ++it;
    }
    return false;
}

void init()
{
    sound_effect_buffers.clear();
}

void clean()
{
    alDeleteBuffers(sound_effect_buffers.size(), sound_effect_buffers.data());

    sound_effect_buffers.clear();
}

} // namespace ns::audio::sound_buffer
