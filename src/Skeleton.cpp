#include "Skeleton.h"

namespace NSEngine {

    Skeleton::Skeleton() {}
    Skeleton::~Skeleton() {}

    Skeleton::Bone* Skeleton::getBone(uint8_t i)
    {
        return &(bones[i]);
    }
    uint8_t Skeleton::nbofbones()
    {
        return bones.size();
    }

    void CalculateAnimationPose(Skeleton *skel, std::vector<glm::mat4>& in, std::vector<glm::mat4>& out)
    {
        std::vector<glm::mat4> localTransform(skel->nbofbones());
        std::vector<glm::mat4> modelTransform(skel->nbofbones());

        for (uint8_t i = 0; i < skel->nbofbones(); i++)
        {
            localTransform[i] = skel->getBone(i)->localMatrix * in[i];
        }

        modelTransform[0] = localTransform[0];

        for (uint8_t i = 1; i < skel->nbofbones(); i++)
        {
            uint8_t parent = skel->getBone(i)->parentID;
            modelTransform[i] = modelTransform[parent] * localTransform[i];
        }

        for (int i = 0; i < skel->nbofbones(); i++)
        {
            out[i] = modelTransform[i] * skel->getBone(i)->invModelMatrix;
        }
    }
}
