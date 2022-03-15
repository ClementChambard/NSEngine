#ifndef ANMFILE_H_
#define ANMFILE_H_

#include <map>
#include <vector>
#include <string>
#include "anmOpener.h"
#include "AnmVM.h"

namespace NSEngine {

    class AnmFile {

        private:
            struct Sprite {
                uint32_t texID = 0; // Right now it uses TextureManager's texID
                float x = 0, y = 0, w = 0, h = 0;
                float u1 = 0, v1 = 0, u2 = 0, v2 = 0;

                void genTexCoords(float w, float h);
            };

        public:
            AnmFile() {}
            AnmFile(std::string const& filename, uint32_t slot);
            void Open(std::string const& filename, uint32_t slot);
            void Cleanup();
            ~AnmFile();

            AnmVM getPreloaded(size_t id) const;
            int8_t* getScript(size_t id) const;
            Sprite getSprite(size_t id) const;
            uint32_t getTextureFromName(std::string const& name) const;
            size_t nbSprites() const { return sprites.size(); }
            size_t nbScripts() const { return scripts.size(); }

        private:
            std::string name = "notLoaded";
            std::map<std::string, uint32_t> textures;
            std::vector<Sprite> sprites;
            std::vector<AnmVM> preloaded;
            std::vector<int8_t*> scripts;

            friend class AnmManagerN;
    };

}

#endif // ANMFILE_H_
