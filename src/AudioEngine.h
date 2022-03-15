#include "SoundDevice.h"
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "MusicBuffer.h"

namespace NSEngine 
{

    class AudioEngine {
        public:
            static void Init();
            static void Clean();
            static void Update();
            static int AddMusic(const char* filename);
            static int AddSFX(const char* filename);
            static int CreateSource();
            static void PlaySoundAtSource(int source, int soundID);
            static void PlaySound(int soundID);
            static void PlayMusic(int musicID);
            static void PauseMusic(int musicID);
            static void StopMusic(int musicID);
            static void ResumeMusic(int musicID);
  
        private:
            static std::vector<SoundSource*> sources;
            static std::vector<SoundSource*> tempSources;
            static std::vector<MusicBuffer*> musics;
            static bool initialized;
    };



}
