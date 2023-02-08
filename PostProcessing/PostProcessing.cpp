#include "PostProcessing.h"

namespace NSEngine { 

    float PostProcessing::POSITIONS[] = {
        -1, 1, -1, -1, 1, 1, 1, -1
    };
    GLuint PostProcessing::vboID = 0;
    GLuint PostProcessing::vaoID = 0;
    std::vector<PPEffect*> PostProcessing::effectsQueue;

}
