#ifndef WATERTILE_HEADER_INCLUDED
#define WATERTILE_HEADER_INCLUDED

#include <defines.h>

namespace ns::n3d {

class WaterTile {
    public:
        WaterTile(f32 center_x, f32 center_z, f32 height) :
            cx(center_x), cz(center_z), y(height) {}

        f32 get_height() const { return y; }
        f32 get_x() const { return cx; }
        f32 get_z() const { return cz; }
        f32 get_w() const { return w; }
        f32 get_h() const { return h; }

        WaterTile* set_size(f32 xscale, f32 zscale) { w = xscale; h = zscale; return this; }

    private:
        f32 cx;
        f32 cz;
        f32 y;
        f32 w = 60;
        f32 h = 60;
};

}

#endif // WATERTILE_HEADER_INCLUDED
