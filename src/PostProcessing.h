#ifndef POSTPROCESSING_INCLUDED_H
#define POSTPROCESSING_INCLUDED_H

#include "NS3D/Loader3D.h"
#include "PPEffect.h"
#include "NSEngine.h"

namespace NSEngine { 

    class PostProcessing {

        public:
            static void init() {
                quad = NS3D::Loader3D::loadToVAO(POSITIONS, 8, 2);
            } 

            static void doPostProcessing(GLuint colorTexture) {
                start();
                for (auto e : effectsQueue)
                {
                    e->render(colorTexture);
                    colorTexture = e->getOutputTexture();
                }
                end();
            }

            static void cleanup() { for (auto e : effectsQueue) { e->cleanup(); delete e; } }

            static void addStep(PPEffect* eff) {
                effectsQueue.push_back(eff);
            }

            static void start() {
                if (engineData::gameflags&0b00000100)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glBindVertexArray(quad->getVaoID());
                glEnableVertexAttribArray(0);
                glDisable(GL_DEPTH_TEST);
            }

            static void end() {
                glEnable(GL_DEPTH_TEST);
                glDisableVertexAttribArray(0);
                glBindVertexArray(0);
                if (engineData::gameflags&0b00000100)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

        private:
            static float POSITIONS[];
            static NS3D::RawModel* quad;
            static std::vector<PPEffect*> effectsQueue;

    };

}

#endif
