#ifndef SOUNDDEVICE
#define SOUNDDEVICE

#include <AL/alc.h>

namespace NSEngine {

    class SoundDevice {
        public:
            static void Init();
            static void Delete();
 
            static void GetLocation(float& x, float& y, float& z);
            static void GetOrientation(float& ori);
            static float GetGain();

            static void SetLocation(const float& x, const float& y, const float& z);
            static void SetOrientation(const float& atx, const float& aty, const float& atz,
                    const float& upx, const float& upy, const float& upz);
            static void SetGain(const float& val);

        private:
            static ALCdevice* device;
            static ALCcontext* context;
    };

}
#endif
