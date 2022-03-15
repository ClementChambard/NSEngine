#ifndef FONTRENDERER_H_
#define FONTRENDERER_H_

#include <vector>
#include "FontShader.h"
#include "RenderableText.h"

namespace NSGui {

    class FontRenderer {

        public:
            FontRenderer() : shader(new FontShader()) {}
            ~FontRenderer() { delete shader; }

            void processText(RenderableText* text) { toRender.push_back(text); }

        private:
            FontShader* shader;

            void prepare();
            void endRendering();

            std::vector<RenderableText*> toRender;

    };

}


#endif // FONTRENDERER_H_
