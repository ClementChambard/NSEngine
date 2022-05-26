#include "NSRig.h"

namespace NSEngine {


    NSRig::NSRig()
    {
        root = new NSRigBone(this);
        bones["root"] = root;
    }

    NSRig::~NSRig()
    {
        delete root;
    }

    void NSRig::Update()
    {
        root->Update();
        time += 1;
    }

    void NSRig::Draw()
    {
        root->Draw();
    }

    NSRigBone* NSRig::getBone(std::string const& name)
    {
        return bones[name];
    }

    void NSRig::addBone(const std::string &parent, const std::string &name, const glm::vec3 &pos1, const glm::vec3 &pos2, const glm::mat4 &mat)
    {
        if (name == "root") return;
        NSRigBone* parentbone;
        parentbone = bones[parent];
        NSRigBone* newbone = new NSRigBone(this, name, parentbone, pos1, pos2, mat);
        parentbone->addChild(newbone);
        bones[name] = newbone;
    }

    void NSRig::setTransform(const glm::mat4 &mat)
    {
        root->setAnimate(mat);
    }

}
