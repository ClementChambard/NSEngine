#ifndef WAV_HEADER_INCLUDED
#define WAV_HEADER_INCLUDED

#include <defines.h>

namespace ns::audio {

enum WAV_AudioFormat : u16 {
    WAV_FMT_INT = 1,
    WAV_FMT_FLOAT = 3,
};

struct WAV_File {
    WAV_File(cstr file_name);
    ~WAV_File();

    WAV_AudioFormat audio_format;
    u16 channel_cnt;
    u32 sample_rate;
    u32 bytes_per_sec;
    u16 bytes_per_block;
    u16 bits_per_sample;
    u32 data_length;
    bytes data;
};

} // namespace ns::audio

#endif // WAV_HEADER_INCLUDED
