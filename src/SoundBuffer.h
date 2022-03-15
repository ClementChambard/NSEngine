#ifndef SOUNDBUFFER
#define SOUNDBUFFER
#include <AL/al.h>
#include <vector>

namespace NSEngine {

    class SoundBuffer {
        public:
            static void Init();
            static void Delete();

            static ALuint addSoundEffect(const char* filename);
            static bool removeSoundEffect(const ALuint& buffer);

        private:
            static std::vector<ALuint> SoundEffectBuffers;
    };

}

#endif
