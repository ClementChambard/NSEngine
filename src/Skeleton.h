#ifndef SKELETON
#define SKELETON

#include <vector>
#include <glm/glm.hpp>

namespace NSEngine {

    class Skeleton {
        private: struct Bone;
        public:
            Skeleton();
            ~Skeleton();
            
            Bone* getBone(uint8_t i);
            uint8_t nbofbones();
        private:
            struct Bone {
                uint8_t parentID;
                glm::mat4 localMatrix;
                glm::mat4 invModelMatrix;
            };
            struct BoneTransformTrack {
                uint8_t boneID = 0;
                std::vector<float> positionsTime;
                std::vector<float> rotationsTime;
                std::vector<float> scalesTime;

                std::vector<glm::vec3> positions;
                std::vector<glm::quat> rotations;
                std::vector<glm::vec3> scales;
            };
            struct Animation {
                float duration = 0.f;
                float ticksPerSecond = 1.0f;
                std::vector<BoneTransformTrack> BoneTransforms;
            };

            std::vector<Bone> bones;
    };

    extern void CalculateAnimationPose(Skeleton* skel, std::vector<glm::mat4>& in, std::vector<glm::mat4>& out);

}

#endif
