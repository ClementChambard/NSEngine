#include "Interpolator.h"
#include "NSEngine.h"

namespace NSEngine {

NSlist<InterpolateManager::InterpParent> InterpolateManager::interpolators;
int InterpolateManager::current = 0;
bool InterpolateManager::delcurrent = false;

bool GameIsPaused()
{
    return engineData::gameflags&0b10000000;
}

void InterpolateManager::DeleteCurrent()
{
    delcurrent = true;
}

void InterpolateManager::Update_All()
{
    current = 0;
    for(auto n = interpolators.front->next; n != interpolators.back; n = n->next)
    {
        n->object->Update();
        if (delcurrent)
        {
            delcurrent = false;
            n = n->previous;
            interpolators.del(current); // XXX: deleting before increment (BAD)
            continue;
        }
        current++;
    }
}

int InterpolateManager::NumOfInterpolators()
{
    return interpolators.size;
}

template<typename T>
T interpolateFunc(T start, T finish, uint16_t duration, uint16_t time, std::function<float(float)> funcgetval)
{
    float i = funcgetval(math::clamp((float)time/(float)duration,0.f,1.f));
    return (T)(start + (i*(finish-start)));
}

template float interpolateFunc(float, float, uint16_t, uint16_t, std::function<float(float)>);
template uint8_t interpolateFunc(uint8_t, uint8_t, uint16_t, uint16_t, std::function<float(float)>);
template glm::vec2 interpolateFunc(glm::vec2, glm::vec2, uint16_t, uint16_t, std::function<float(float)>);
template glm::vec3 interpolateFunc(glm::vec3, glm::vec3, uint16_t, uint16_t, std::function<float(float)>);
template glm::vec4 interpolateFunc(glm::vec4, glm::vec4, uint16_t, uint16_t, std::function<float(float)>);
template NSEngine::Color interpolateFunc(NSEngine::Color, NSEngine::Color, uint16_t, uint16_t, std::function<float(float)>);
template NSEngine::ColorRGB interpolateFunc(NSEngine::ColorRGB, NSEngine::ColorRGB, uint16_t, uint16_t, std::function<float(float)>);
template NSEngine::NSITPuint8_t interpolateFunc(NSEngine::NSITPuint8_t, NSEngine::NSITPuint8_t, uint16_t, uint16_t, std::function<float(float)>);

}
