#ifndef SOUNDSOURCE
#define SOUNDSOURCE
#include <AL/al.h>

namespace NSEngine {

    class SoundSource {
        public:
            SoundSource();
            ~SoundSource();

            void Play(const ALuint& buffer_to_play);
            void Stop();
            void Pause();
            void Resume();

            void SetBufferToPlay(const ALuint& buffer_to_play);
            void SetLooping(const bool& loop);

            bool isPlaying();

        private:
            ALuint Source;
            float Pitch = 1.f;
            float Gain = 1.f;
            float Position[3] = { 0, 0, 0 };
            float Velocity[3] = { 0, 0, 0 };
            bool LoopSound = false;
            ALuint Buffer = 0;
    };

}

#endif
