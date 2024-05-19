#ifndef WATERTILE_INCLUDED_H
#define WATERTILE_INCLUDED_H

#include "../defines.h"

namespace NS3D {

class WaterTile {
    public:
        WaterTile(f32 centerX, f32 centerZ, f32 height) :
            cX(centerX), cZ(centerZ), y(height) {}

        f32 getHeight() const { return y; }
        f32 getX() const { return cX; }
        f32 getZ() const { return cZ; }
        f32 getW() const { return w; }
        f32 getH() const { return h; }

        WaterTile* setSize(f32 xscale, f32 zscale) { w = xscale; h = zscale; return this; }

    private:
        f32 cX;
        f32 cZ;
        f32 y;
        f32 w = 60;
        f32 h = 60;
};

}

#endif
