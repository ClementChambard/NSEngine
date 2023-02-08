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
            Light(NSEngine::Bytestream const& bytes, uint32_t offset)
            {
                position.x = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 0));
                position.y = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 4));
                position.z = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 8));
                color.r = (float) NSEngine::getUint8(bytes, (NSEngine::Offset) (offset+12)) / 255.f;
                color.g = (float) NSEngine::getUint8(bytes, (NSEngine::Offset) (offset+13)) / 255.f;
                color.b = (float) NSEngine::getUint8(bytes, (NSEngine::Offset) (offset+14)) / 255.f;
                attenuation.x = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 15));
                attenuation.y = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 19));
                attenuation.z = NSEngine::getFloat(bytes, (NSEngine::Offset) (offset + 23));
            }

            glm::vec3 getPosition() const { return position; }
            glm::vec3 getColor() const { return color; }
            glm::vec3 getAttenuation() const { return attenuation; }
            void setPosition(const glm::vec3& pos) { position = pos; }
            void setColor(const glm::vec3& col) { color = col; }
            void setAttenuation(const glm::vec3& att) { attenuation = att; }

            NSEngine::Bytestream parseBytes()
            {
                NSEngine::Bytestream out;
                NSEngine::insertBytestream(out, position.x);
                NSEngine::insertBytestream(out, position.y);
                NSEngine::insertBytestream(out, position.z);
                NSEngine::insertBytestream(out, (uint8_t) (color.r*255));
                NSEngine::insertBytestream(out, (uint8_t) (color.g*255));
                NSEngine::insertBytestream(out, (uint8_t) (color.b*255));
                NSEngine::insertBytestream(out, attenuation.x);
                NSEngine::insertBytestream(out, attenuation.y);
                NSEngine::insertBytestream(out, attenuation.z);
                return out;
            }
            void parseBytes(NSEngine::Bytestream& bs)
            {
                NSEngine::insertBytestream(bs, position.x);
                NSEngine::insertBytestream(bs, position.y);
                NSEngine::insertBytestream(bs, position.z);
                NSEngine::insertBytestream(bs, (uint8_t) (color.r*255));
                NSEngine::insertBytestream(bs, (uint8_t) (color.g*255));
                NSEngine::insertBytestream(bs, (uint8_t) (color.b*255));
                NSEngine::insertBytestream(bs, attenuation.x);
                NSEngine::insertBytestream(bs, attenuation.y);
                NSEngine::insertBytestream(bs, attenuation.z);
            }

        private:
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 attenuation = glm::vec3(1,0,0);
    };

}

#endif
