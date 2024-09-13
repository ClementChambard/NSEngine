#ifndef MUSICBUFFER_HEADER_INCLUDED
#define MUSICBUFFER_HEADER_INCLUDED

#include <AL/al.h>
#include <sndfile.h>
#include <defines.h>

namespace ns::audio {

class MusicBuffer {
    public:
        void play();
        void pause();
        void stop();
        void resume();

        void update_buffer_stream();

        ALint get_source();
        bool is_playing();

        MusicBuffer(cstr filename);
        ~MusicBuffer();
    private:
        ALuint source;
        static const int BUFFER_SAMPLES = 8192;
        static const int NUM_BUFFERS = 4;
        ALuint buffers[NUM_BUFFERS];
        SNDFILE* snd_file;
        SF_INFO sf_info;
        i16* membuff;
        ALenum format;
};

} // namespace ns::audio

#endif // MUSICBUFFER_HEADER_INCLUDED
