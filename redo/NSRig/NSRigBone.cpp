#include "NSRigBone.h"

#include "../math/math.h"
#include "../DrawFuncs.h"
#include "NSRigGraphicsLine.h"
#include "NSRig.h"

namespace NSEngine {

    NSRigBone::NSRigBone(NSRig* rig, std::string name, NSRigBone* parent, glm::mat4 const& mat)
    {
        this->rig = rig;
        this->name = name;
        this->parent = parent;
        propagate = mat;
    }

    NSRigBone::NSRigBone(NSRig* rig, std::string name, NSRigBone* parent, glm::vec3 const& pos1, glm::vec3 const& pos2, glm::mat4 const& mat) : NSRigBone(rig, name, parent, mat)
    {
        addGraphics((NSRigGraphics*) new NSRigGraphicsLine(pos1, pos2));
    }

    NSRigBone::NSRigBone(NSRig* rig)
    {
        this->rig = rig;
        name = "root";
    }

    NSRigBone::~NSRigBone()
    {
        for (NSRigBone* b : childs) delete b;
        childs.clear();
    }

    void NSRigBone::addChild(NSRigBone *b)
    {
        childs.push_back(b);
    }

    void NSRigBone::Update()
    {
        if (name != "root") animate = rig->getAnim(name);
        self = rig->topMatrix() * animate;
        rig->pushMatrix(self * propagate);
        for (NSRigBone* b : childs) b->Update();
        rig->popMatrix();
    }

    void NSRigBone::Draw()
    {
        for (NSRigGraphics* g : graphics) g->Draw(self);
        for (NSRigBone* b : childs) b->Draw();
    }

}
