#ifndef CAMERA2D
#define CAMERA2D

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NS_CAM_2D 0

namespace NSEngine {

    class Camera2D {
        public:
            Camera2D(int screenW, int screenH);
            ~Camera2D();

            void setPosition(const glm::vec3& pos);
            void setScale(const float& scal);

            glm::vec3 getPosition();
            glm::vec3 getPositionTo();
            float getScale();
            glm::vec2 getSize();

            void Update();

            glm::mat4 getCam();
            glm::mat4 getCamStatic();
            glm::mat4 getCamGUI();
            glm::vec4 getBounds();

        private:
            int scr_w = 0, scr_h = 0;
            bool needs_changes = true;
            float scale = 1.f;
            float scaleto = scale;
            glm::vec3 position;
            glm::vec3 positionto = position;
            glm::mat4 orthoMatrix;
            glm::mat4 cameraMatrix;

    };

}

#endif
