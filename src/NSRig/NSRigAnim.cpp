#include "NSRigAnim.h"
#include "../Interpolator_impl.h"
#include <glm/gtc/matrix_transform.hpp>

namespace NSEngine {

    NSRigAnim::NSRigAnim()
    { }

    NSRigAnim::~NSRigAnim() {}

    glm::mat4 NSRigAnim::getInterpolatedMatrix(std::string const& bone, float& time, NSRigAnim::KeyFrame* &lastKeyFrame)
    {
        if (time >= duration) time -= duration;
        KeyFrame* next = nextKeyFrame(bone, time);
        if (lastKeyFrame == nullptr || next->time == time)
        {
            lastKeyFrame = next;
            return next==nullptr?glm::mat4(1.f):makeMat(next->pos, next->rot, next->scale);
        }
        if (next == nullptr) return glm::mat4(1.f);
        /* get modified time value between 0 and 1 */
        float t;
        if (next->time > lastKeyFrame->time) t = (time - lastKeyFrame->time) / (next->time - lastKeyFrame->time);
        else
        {
            float dt, len = duration - lastKeyFrame->time + next->time;
            if (time > lastKeyFrame->time) dt = time - lastKeyFrame->time;
            else dt = time + len - next->time;
            t = dt / len;
        }
        /* modify t depending on interpolation mode */
        t = FuncGetVal(next->interpolationMode)(t);
        /* interpolate */
        glm::vec3 pos   = lastKeyFrame->pos   + (next->pos   - lastKeyFrame->pos  ) * t;
        glm::vec3 rot   = lastKeyFrame->rot   + (next->rot   - lastKeyFrame->rot  ) * t;
        glm::vec3 scale = lastKeyFrame->scale + (next->scale - lastKeyFrame->scale) * t;
        return makeMat(pos, rot, scale);
    }

    glm::mat4 NSRigAnim::makeMat(glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale)
    {
        return glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.f), pos), rot.z, glm::vec3(0,0,1)), rot.y, glm::vec3(0,1,0)), rot.x, glm::vec3(1,0,0)), scale);
    }

    NSRigAnim::KeyFrame* NSRigAnim::nextKeyFrame(std::string const& bone, float time)
    {
        float timeDiff = 10000000.f;
        float timeFirst = 10000000.f;
        KeyFrame* next = nullptr;
        KeyFrame* first = nullptr;
        for (KeyFrame& k : keyFrames)
        {
            if (k.boneName != bone) continue;
            if (k.time < timeFirst)
            {
                timeFirst = k.time;
                first = &k;
            }
            float dt = k.time - time;
            if (dt >= 0 && dt < timeDiff)
            {
                timeDiff = dt;
                next = &k;
            }
        }
        if (next == nullptr && type == 0) return first;
        return next;
    }

}
