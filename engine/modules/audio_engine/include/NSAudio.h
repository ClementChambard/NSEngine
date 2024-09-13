#ifndef NSAUDIO_HEADER_INCLUDED
#define NSAUDIO_HEADER_INCLUDED

#include <defines.h>

namespace ns::audio {

u32 add_music(cstr filename);

u32 add_sfx(cstr filename);

u32 create_source();

void play_sound_at_source(u32 source, u32 sound_id);

void play_sound(u32 sound_id);

void play_music(u32 music_id);

void pause_music(u32 music_id);

void stop_music(u32 music_id);

void resume_music(u32 music_id);

} // namespace ns::audio

#endif // NSAUDIO_HEADER_INCLUDED
