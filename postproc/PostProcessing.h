#ifndef POSTPROCESSING_INCLUDED_H
#define POSTPROCESSING_INCLUDED_H

#include "PPEffect.h"
#include "NSEngine.hpp"
#include "./defines.h"

namespace ns { 

class PostProcessing {

    public:
        static void init() {
            glGenVertexArrays(1, &vaoID);
            glBindVertexArray(vaoID);
            glGenBuffers(1, &vboID);
            glBindBuffer(GL_ARRAY_BUFFER, vboID);
            glBufferData(GL_ARRAY_BUFFER, 8*sizeof(f32), POSITIONS, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
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

        static void cleanup() {
            for (auto e : effectsQueue) { e->cleanup(); delete e; }
            glDeleteBuffers(1, &vboID);
            glDeleteVertexArrays(1, &vaoID);
        }

        static void addStep(PPEffect* eff) {
            effectsQueue.push_back(eff);
        }

        static void start() {
            if (getInstance()->flags().flags.wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBindVertexArray(vaoID);
            glEnableVertexAttribArray(0);
            glDisable(GL_DEPTH_TEST);
        }

        static void end() {
            glEnable(GL_DEPTH_TEST);
            glDisableVertexAttribArray(0);
            glBindVertexArray(0);
            if (getInstance()->flags().flags.wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

    private:
        static f32 POSITIONS[];
        static GLuint vaoID;
        static GLuint vboID;
        static std::vector<PPEffect*> effectsQueue;

};

}

#endif
