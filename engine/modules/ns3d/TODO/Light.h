#ifndef LIGHT_INCLUDED_H
#define LIGHT_INCLUDED_H

#include <glm/glm.hpp>
#include "../Bytestreams.h"

#define MAX_LIGHTS 4

namespace NS3D {

class Light {
    public:
        Light(const glm::vec3& pos, const glm::vec3& col) :
            position(pos), color(col) {}
        Light(const glm::vec3& pos, const glm::vec3& col, const glm::vec3& att) :
            position(pos), color(col), attenuation(att) {}
        /**
         * create a light from bytes
         */
        Light(ns::Bytestream const& bytes, u32 offset)
        {
            position.x = ns::getFloat(bytes, (ns::Offset) (offset + 0));
            position.y = ns::getFloat(bytes, (ns::Offset) (offset + 4));
            position.z = ns::getFloat(bytes, (ns::Offset) (offset + 8));
            color.r = (f32) ns::getUint8(bytes, (ns::Offset) (offset+12)) / 255.f;
            color.g = (f32) ns::getUint8(bytes, (ns::Offset) (offset+13)) / 255.f;
            color.b = (f32) ns::getUint8(bytes, (ns::Offset) (offset+14)) / 255.f;
            attenuation.x = ns::getFloat(bytes, (ns::Offset) (offset + 15));
            attenuation.y = ns::getFloat(bytes, (ns::Offset) (offset + 19));
            attenuation.z = ns::getFloat(bytes, (ns::Offset) (offset + 23));
        }

        glm::vec3 getPosition() const { return position; }
        glm::vec3 getColor() const { return color; }
        glm::vec3 getAttenuation() const { return attenuation; }
        void setPosition(const glm::vec3& pos) { position = pos; }
        void setColor(const glm::vec3& col) { color = col; }
        void setAttenuation(const glm::vec3& att) { attenuation = att; }

        ns::Bytestream parseBytes()
        {
            ns::Bytestream out;
            ns::insertBytestream(out, position.x);
            ns::insertBytestream(out, position.y);
            ns::insertBytestream(out, position.z);
            ns::insertBytestream(out, (u8) (color.r*255));
            ns::insertBytestream(out, (u8) (color.g*255));
            ns::insertBytestream(out, (u8) (color.b*255));
            ns::insertBytestream(out, attenuation.x);
            ns::insertBytestream(out, attenuation.y);
            ns::insertBytestream(out, attenuation.z);
            return out;
        }
        void parseBytes(ns::Bytestream& bs)
        {
            ns::insertBytestream(bs, position.x);
            ns::insertBytestream(bs, position.y);
            ns::insertBytestream(bs, position.z);
            ns::insertBytestream(bs, (u8) (color.r*255));
            ns::insertBytestream(bs, (u8) (color.g*255));
            ns::insertBytestream(bs, (u8) (color.b*255));
            ns::insertBytestream(bs, attenuation.x);
            ns::insertBytestream(bs, attenuation.y);
            ns::insertBytestream(bs, attenuation.z);
        }

    private:
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 attenuation = glm::vec3(1,0,0);
};

}

#endif
