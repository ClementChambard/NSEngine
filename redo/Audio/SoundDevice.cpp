#include "SoundDevice.h"
#include "Error.h"
#include <AL/al.h>
#include <vector>

namespace NSEngine {

    ALCdevice* SoundDevice::device;
    ALCcontext* SoundDevice::context;

    void SoundDevice::Init() 
    {
        device = alcOpenDevice(nullptr);
        if (!device) 
            fatalError("Failed to get sound device");
    
        context = alcCreateContext(device, nullptr);
        if (!context) 
            fatalError("Failed to set sound context");

        if (!alcMakeContextCurrent(context)) 
            fatalError("failed to make context current");

        const ALCchar* name = nullptr;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(device) != ALC_NO_ERROR)
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        info("Opened sound device :", name);
    }

    void SoundDevice::Delete() 
    {
        if (!alcMakeContextCurrent(nullptr))
            fatalError("failed to set context to nullptr");

        alcDestroyContext(context);
        //if (context) error("failed to unset audio context");

        if (!alcCloseDevice(device))
            fatalError("failed to close sound device");
    }

    void SoundDevice::GetLocation(float& x, float& y, float& z) { alGetListener3f(AL_POSITION, &x, &y, &z); }
    void SoundDevice::GetOrientation(float& ori) { alGetListenerfv(AL_ORIENTATION, &ori); }
    float SoundDevice::GetGain() { float gain; alGetListenerf(AL_GAIN, &gain); return gain; }

    void SoundDevice::SetLocation(const float& x, const float& y, const float& z) { alListener3f(AL_POSITION, x, y, z); }
    void SoundDevice::SetOrientation(const float& atx, const float& aty, const float& atz, const float& upx, const float& upy, const float& upz)
    {
        std::vector<float> ori;
        ori.push_back(atx);
        ori.push_back(aty);
        ori.push_back(atz);
        ori.push_back(upx);
        ori.push_back(upy);
        ori.push_back(upz);
        alGetListenerfv(AL_ORIENTATION, ori.data());
    }
    void SoundDevice::SetGain(const float& gain) { float g = gain; if (g<0) g=0; if (g>5) g=5; alListenerf(AL_GAIN, g); }

}
