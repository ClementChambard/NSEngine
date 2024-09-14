#ifndef LIGHT_HEADER_INCLUDED
#define LIGHT_HEADER_INCLUDED

#include <defines.h>
#include <math/types/types.hpp>

namespace ns::n3d {

class Light {
    public:
        Light(const vec3& pos, const vec3& col) :
            position(pos), color(col) {}
        Light(const vec3& pos, const vec3& col, const vec3& att) :
            position(pos), color(col), attenuation(att) {}

        vec3 get_position() const { return position; }
        vec3 get_color() const { return color; }
        vec3 get_attenuation() const { return attenuation; }
        void set_position(const vec3& pos) { position = pos; }
        void set_color(const vec3& col) { color = col; }
        void set_attenuation(const vec3& att) { attenuation = att; }

        static constexpr u32 MAX_LIGHTS = 4;

    private:
        vec3 position;
        vec3 color;
        vec3 attenuation = vec3(1,0,0);
};

}

#endif // LIGHT_HEADER_INCLUDED
