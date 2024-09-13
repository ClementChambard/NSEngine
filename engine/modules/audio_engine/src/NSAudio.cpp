#include <NSAudio.h>
#include "SoundBuffer.h"
#include "SoundSource.h"
#include "SoundDevice.h"
#include "MusicBuffer.h"

namespace ns::audio {

static std::vector<Source*> sources;
static std::vector<Source*> temp_sources;
static std::vector<MusicBuffer*> musics;
static bool initialized;

void init()
{
    if (initialized) return;
    device::init();
    sound_buffer::init();
    initialized = true;
}

void clean()
{
    if (!initialized) return;
    initialized = false;
    for (auto s : sources) delete s;
    for (auto m : musics) delete m;
    sound_buffer::clean();
    device::clean();
}

void update()
{
    if (!initialized) return;
    for (auto m : musics) { if (m->is_playing()) m->update_buffer_stream(); }
    for (u32 i = 0; i < temp_sources.size(); i++)
    {
        if (temp_sources[i]!=nullptr && !temp_sources[i]->is_playing())
        {
            delete temp_sources[i]; temp_sources[i] = nullptr;
            if (i == temp_sources.size()-1)
            {
                int j = i;
                while(temp_sources[j]==nullptr && j-->=0) temp_sources.pop_back();
            }
        }
    }
}

void play_sound(u32 sound_id)
{
    int i = temp_sources.size();
    temp_sources.push_back(new Source());
    temp_sources[i]->play(sound_id);
}

u32 create_source()
{
    if (!initialized) return -1;
    sources.push_back(new Source());
    return sources.size()-1;
}

void play_sound_at_source(u32 source, u32 sound_id)
{
    if (!initialized) return;
    sources[source]->play(sound_id);
}

u32 add_sfx(cstr filename)
{
    if (!initialized) return -1;
    return sound_buffer::add_sound_effect(filename);
}

u32 add_music(cstr filename)
{
    if (!initialized) return -1;
    musics.push_back(new MusicBuffer(filename));
    return musics.size() -1;
}

void play_music(u32 music_id)
{
    if (!initialized) return;
    musics[music_id]->play();
}

void pause_music(u32 music_id)
{
    if (!initialized) return;
    musics[music_id]->pause();
}

void stop_music(u32 music_id)
{
    if (!initialized) return;
    musics[music_id]->stop();
}

void resume_music(u32 music_id)
{
    if (!initialized) return;
    musics[music_id]->resume();
}

} // namespace ns::audio
