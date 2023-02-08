#ifndef OBJECT3D_INCLUDED_H
#define OBJECT3D_INCLUDED_H

#include "TexturedModel.h"
#include "../math/math.h"
#include <glm/glm.hpp>

namespace NS3D {

    class Object3D {
        public:

            Object3D() {}
            Object3D(TexturedModel* m, const glm::vec3& p, const glm::vec3& r, const glm::vec3& s) :
                model(m), position(p), rotation(r), scale(s) {}
            Object3D(TexturedModel* m, const int i, const glm::vec3& p, const glm::vec3& r, const glm::vec3& s) :
                model(m), textureIndex(i), position(p), rotation(r), scale(s) {}

            float getTextureXOffset() const { return (float)(textureIndex%model->getModelTexture()->getNumberOfRows())/(float)model->getModelTexture()->getNumberOfRows();}
            float getTextureYOffset() const { return (float)(textureIndex/model->getModelTexture()->getNumberOfRows())/(float)model->getModelTexture()->getNumberOfRows();}

            TexturedModel* getModel() const { return model; }
            glm::vec3 getPosition() const { return position; }
            glm::vec3 getRotation() const { return rotation; }
            glm::vec3 getScale() const { return scale; }
            glm::mat4 getMatrix() const { return math::makeTransformationMatrix(position, rotation, scale); }
            int getTextureIndex() const { return textureIndex; }

            void setPosition(const glm::vec3& pos) { position = pos; }
            void incPosition(const glm::vec3& pos) { position += pos; }
            void setRotation(const glm::vec3& rot) { rotation = rot; }
            void incRotation(const glm::vec3& rot) { rotation += rot; }
            void setScale(const glm::vec3& sca) { scale = sca; }
            void incScale(const glm::vec3& sca) { scale += sca; }
            void setTextureIndex(const int i) { textureIndex = i; }
            void setTexturedModel(TexturedModel* m) { model = m; }

        private:
            TexturedModel* model = nullptr;
            glm::vec3 position = {0, 0, 0};
            glm::vec3 rotation = {0, 0, 0};
            glm::vec3 scale = {1, 1, 1};
            int textureIndex = 0;
    };

}

#endif
