#ifndef WATERTILE_INCLUDED_H
#define WATERTILE_INCLUDED_H

namespace NS3D {

    class WaterTile {
        public:
            WaterTile(float centerX, float centerZ, float height) :
                cX(centerX), cZ(centerZ), y(height) {}

            float getHeight() const { return y; }
            float getX() const { return cX; }
            float getZ() const { return cZ; }
            float getW() const { return w; }
            float getH() const { return h; }

            WaterTile* setSize(float xscale, float zscale) { w = xscale; h = zscale; return this; }

        private:
            float y;
            float cX;
            float cZ;
            float w = 60;
            float h = 60;
    };

}

#endif
