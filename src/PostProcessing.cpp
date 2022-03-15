#include "PostProcessing.h"

namespace NSEngine { 

    float PostProcessing::POSITIONS[] = {
        -1, 1, -1, -1, 1, 1, 1, -1
    };
    NS3D::RawModel* PostProcessing::quad;
    std::vector<PPEffect*> PostProcessing::effectsQueue;

}
