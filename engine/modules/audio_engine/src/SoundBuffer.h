#ifndef SOUNDBUFFER_HEADER_INCLUDED
#define SOUNDBUFFER_HEADER_INCLUDED
#include <AL/al.h>
#include <vector>
#include <defines.h>

namespace ns::audio::sound_buffer {

void init();
void clean();

u32 add_sound_effect(cstr filename);
bool remove_sound_effect(u32 buffer);

} // namespace ns::audio::sound_buffer

#endif // SOUNDBUFFER_HEADER_INCLUDED
