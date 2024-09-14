#ifndef WATERRENDERER_HEADER_INCLUDED
#define WATERRENDERER_HEADER_INCLUDED

#include "RawModel.h"
#include <Texture.hpp>
#include "WaterTile.h"
#include "WaterShader.h"
#include <vector>

namespace ns::n3d {

class WaterRenderer {
    public:
        WaterRenderer(WaterShader* shad);
        ~WaterRenderer();

        void render(const std::vector<const WaterTile*>& water, const Light* sun);

    private:
        WaterShader* shader;
        RawModel* quad;
        Texture* dudv_map_tex;
        Texture* normal_map_tex;

        void prepare_render(const Light* sun);
        void unbind();
        void setup_vao();
};

}

#endif // WATERRENDERER_HEADER_INCLUDED
