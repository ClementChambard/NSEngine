#ifndef SOUNDSOURCE_HEADER_INCLUDED
#define SOUNDSOURCE_HEADER_INCLUDED

#include <defines.h>

namespace ns::audio {

class Source {
    public:
        Source();
        ~Source();

        void play(u32 buffer_to_play);
        void stop();
        void pause();
        void resume();

        void set_buffer_to_play(u32 buffer_to_play);
        void set_looping(bool loop);

        bool is_playing();

    private:
        u32 source;
        f32 pitch = 1.f;
        f32 gain = 1.f;
        f32 position[3] = { 0, 0, 0 };
        f32 velocity[3] = { 0, 0, 0 };
        bool loop_sound = false;
        u32 buffer = 0;
};

} // namespace ns::audio

#endif // SOUNDSOURCE_HEADER_INCLUDED
