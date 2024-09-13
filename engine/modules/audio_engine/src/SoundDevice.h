#ifndef SOUNDDEVICE_HEADER_INCLUDED
#define SOUNDDEVICE_HEADER_INCLUDED

#include <defines.h>

namespace ns::audio::device {

void init();
void clean();

void get_location(f32& x, f32& y, f32& z);
void get_orientation(f32& ori);
f32 get_gain();

void set_location(f32 x, f32 y, f32 z);
void set_orientation(f32 atx, f32 aty, f32 atz, f32 upx, f32 upy, f32 upz);
void set_gain(f32 val);

} // namespace ns::audio::device

#endif // SOUNDDEVICE_HEADER_INCLUDED
