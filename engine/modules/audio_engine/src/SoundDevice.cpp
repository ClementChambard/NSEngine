#include "SoundDevice.h"
#include "logger.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

namespace ns::audio::device {

static ALCdevice* device;
static ALCcontext* context;

void init()
{
    device = alcOpenDevice(nullptr);
    if (!device)
        NS_FATAL("Failed to get sound device");

    context = alcCreateContext(device, nullptr);
    if (!context)
        NS_FATAL("Failed to set sound context");

    if (!alcMakeContextCurrent(context))
        NS_FATAL("failed to make context current");

    const ALCchar* name = nullptr;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != ALC_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    NS_INFO("Opened sound device : %s", name);
}

void clean()
{
    if (!alcMakeContextCurrent(nullptr))
        NS_FATAL("failed to set context to nullptr");

    alcDestroyContext(context);
    //if (context) error("failed to unset audio context");

    if (!alcCloseDevice(device))
        NS_FATAL("failed to close sound device");
}

void get_location(f32& x, f32& y, f32& z) { alGetListener3f(AL_POSITION, &x, &y, &z); }
void get_orientation(f32& ori) { alGetListenerfv(AL_ORIENTATION, &ori); }
f32 get_gain() { f32 gain; alGetListenerf(AL_GAIN, &gain); return gain; }

void set_location(f32 x, f32 y, f32 z) { alListener3f(AL_POSITION, x, y, z); }
void set_orientation(f32 atx, f32 aty, f32 atz, f32 upx, f32 upy, f32 upz)
{
    std::vector<f32> ori;
    ori.push_back(atx);
    ori.push_back(aty);
    ori.push_back(atz);
    ori.push_back(upx);
    ori.push_back(upy);
    ori.push_back(upz);
    alGetListenerfv(AL_ORIENTATION, ori.data());
}
void set_gain(f32 gain) { f32 g = gain; if (g<0) g=0; if (g>5) g=5; alListenerf(AL_GAIN, g); }

} // namespace ns::audio::device
