#ifndef POSTPROCESSING_HEADER_INCLUDED
#define POSTPROCESSING_HEADER_INCLUDED

#include "PPEffect.h"
#include "./defines.h"
#include <vector>

namespace ns { 

class PostProcessing {

    public:
        static void init();
        static void do_post_processing(u32 color_texture);
        static void cleanup();
        static void add_step(PPEffect* eff);
        static void start();
        static void end();

    private:
        static const f32 POSITIONS[];
        static u32 vao_id;
        static u32 vbo_id;
        static std::vector<PPEffect*> effects_queue;

};

}

#endif // POSTPROCESSING_HEADER_INCLUDED
