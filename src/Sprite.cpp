#include "Sprite.h"
#include "FileOpener.h"
#include "NSEngine.h"
#include "SpriteBatch.h"
#include "TextureManager.h"
#include "vertex.h"
#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <cstddef>

#ifndef transfPos1
#define transfPos1(a) {a.position.x * xs, a.position.y * ys, a.position.z}
#endif
#ifndef transfPos2
#define transfPos2(a) {a.x + x, a.y + y, a.z}
#endif

namespace NSEngine {

    int Sprite::blendmode = 0;

    Sprite::Sprite() {}
    Sprite::~Sprite() {}

    Sprite* Sprite::LoadSprite(const char *name)
    {
        FileOpener::openFile(name);
        texId = FileOpener::readUint16(0);
        numberOfFrames = 0;
        int fr = FileOpener::readUint16(2);
        for (int i = 0; i < fr; i++)
        {
            int off = i*14;
            frameDuration.push_back(FileOpener::readUint16(4+off));
            numberOfFrames += frameDuration[i];
            int ox_ = FileOpener::readInt16(6+off);
            int oy_ = FileOpener::readInt16(8+off);
            int x1_ = FileOpener::readUint16(10+off);
            int y1_ = FileOpener::readUint16(12+off);
            int x2_ = FileOpener::readUint16(14+off);
            int y2_ = FileOpener::readUint16(16+off);
            int w_ = x2_ - x1_;
            int h_ = y2_ - y1_;
            UV uv1_ = TextureManager::GetUVAt(texId, x1_, y1_);
            UV uv2_ = TextureManager::GetUVAt(texId, x2_, y2_);
            ConstructFrame(w_, h_, ox_, oy_, uv1_, uv2_);
        }
        FileOpener::closeFile();
        return this;
    }

    void Sprite::DrawFrame(int frame, int x, int y, float xs, float ys, float rot, Color tl, Color tr, Color br, Color bl)
    {

        frame %= frames.size();

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), rot, {0.f, 0.f, 1.f});

        glm::vec3 tl1 = transfPos1(frames[frame].tl);
        glm::vec4 rotatedTL = glm::vec4(tl1,1.f) * rotationMatrix;
        glm::vec3 tr1 = transfPos1(frames[frame].tr);
        glm::vec4 rotatedTR = glm::vec4(tr1,1.f) * rotationMatrix;
        glm::vec3 br1 = transfPos1(frames[frame].br);
        glm::vec4 rotatedBR = glm::vec4(br1,1.f) * rotationMatrix;
        glm::vec3 bl1 = transfPos1(frames[frame].bl);
        glm::vec4 rotatedBL = glm::vec4(bl1,1.f) * rotationMatrix;

        Vertex TL = {transfPos2(rotatedTL), tl, frames[frame].tl.uv};
        Vertex TR = {transfPos2(rotatedTR), tl, frames[frame].tr.uv};
        Vertex BR = {transfPos2(rotatedBR), tl, frames[frame].br.uv};
        Vertex BL = {transfPos2(rotatedBL), tl, frames[frame].bl.uv};

        engineData::layers[engineData::targetLayer]->getBatch()->draw(texId, TL, TR, BR, BL, blendmode);
    }

    void Sprite::ConstructFrame(int width, int height, int offsetx, int offsety, UV uv1, UV uv2)
    {

        float x = -offsetx;
        float y = -(height - offsety);
        float w = width;
        float h = height;

        if (w < 0 && h > 0) 
        {  
            frames.push_back(
            {   {{  x,y-w,0}, c_white, {uv1.u,uv1.v}},
                {{x+h,y-w,0}, c_white, {uv1.u,uv2.v}},
                {{x+h,  y,0}, c_white, {uv2.u,uv2.v}},
                {{  x,  y,0}, c_white, {uv2.u,uv1.v}} }
            ); return;
        }
        frames.push_back(
        {   {{  x,y+h,0}, c_white, {uv1.u,uv1.v}},
            {{x+w,y+h,0}, c_white, {uv2.u,uv1.v}},
            {{x+w,  y,0}, c_white, {uv2.u,uv2.v}},
            {{  x,  y,0}, c_white, {uv1.u,uv2.v}} }
        );
    }

    int Sprite::getImageIndex(float frame)
    {
        int f = floor(frame);
        frame-= f;
        f %= numberOfFrames;
        frame += f;
        for (auto i = 0; i < frameDuration.size(); i++)
        {
            if (frame < frameDuration[i]) return i;
            frame -= frameDuration[i];
        }
        return 0;
    }
}
