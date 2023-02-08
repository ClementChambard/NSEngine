#ifndef NSRIGANIM_H_
#define NSRIGANIM_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace NSEngine {

    class NSRigAnim {

        public:
            NSRigAnim();
            ~NSRigAnim();

            struct KeyFrame {
                KeyFrame(std::string const& b, float t, glm::vec3 const& p, glm::vec3 const& r, glm::vec3 const& s, uint8_t i) :
                    boneName(b), time(t), pos(p), rot(r), scale(s), interpolationMode(i) {}
                std::string boneName = "";
                float time = 0;
                glm::vec3 pos = {0,0,0};
                glm::vec3 rot = {0,0,0};
                glm::vec3 scale = {0,0,0};
                uint8_t interpolationMode = 0;
            };

            void setDuration(float d) { duration = d; }
            void addKeyFrame(KeyFrame k) { keyFrames.push_back(k); }

            glm::mat4 getInterpolatedMatrix(std::string const& bone, float& time, KeyFrame* &lastKeyFrame);

        private:
            KeyFrame* nextKeyFrame(std::string const& bone, float time);
            glm::mat4 makeMat(glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& scale);

            float duration;
            int type = 0;
            std::vector<KeyFrame> keyFrames;
    };

}

#endif // NSRIGANIM_H_
