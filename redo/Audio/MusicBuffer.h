#ifndef MUSICBUFFER
#define MUSICBUFFER

#include <AL/al.h>
#include <sndfile.h>

namespace NSEngine {

    class MusicBuffer {
        public:
            void Play();
            void Pause();
            void Stop();
            void Resume();

            void UpdateBufferStream();

            ALint getSource();
            bool isPlaying();

            MusicBuffer(const char* filename);
            ~MusicBuffer();
        private:
            ALuint Source;
            static const int BUFFER_SAMPLES = 8192;
            static const int NUM_BUFFERS = 4;
            ALuint Buffers[NUM_BUFFERS];
            SNDFILE* SndFile;
            SF_INFO SfInfo;
            short* Membuff;
            ALenum Format;
    };

}

#endif
