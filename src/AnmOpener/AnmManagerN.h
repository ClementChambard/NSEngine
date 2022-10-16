#ifndef ANMMANAGERN_H_
#define ANMMANAGERN_H_

#include "AnmFile.h"
#include <array>

namespace NSEngine
{

    class AnmManagerN {

        public:

            static void Init();
            static void Cleanup();

            static uint32_t SpawnVM(size_t slot, size_t script, bool ui = false, bool front = false);
            static AnmVM* SpawnVMExt(size_t slot, size_t script);

            static AnmVM* getVM(uint32_t id);

            static void deleteVM(uint32_t id);
            static bool isAlive(uint32_t id);
            static void LoadFile(size_t slot, std::string filename);

            static void update(bool printInstr = false);
            static void draw();
            static void on_tick_world();
            static void on_tick_ui();
            static void on_draw(uint32_t layer);

            static void drawSprite(size_t slot, size_t& spriteID);
            static void drawTexture(size_t slot, size_t& texID);

        private:

            //216 unknown bytes + int32 useless_count

            static AnmVMList* first;
            static AnmVMList* last;
            static AnmVMList* uiFirst;
            static AnmVMList* uiLast;

            static AnmFastVM fastArray[8191];

            //lolk fields

            static AnmFastVMList* fastFirst;
            //int32 unknown

            static std::array<AnmFile, 31> loadedFiles;

            static int32_t last_id_discriminator;

            static bool initialized;

            friend class AnmVM;

    };

}


#endif // ANMMANAGERN_H_
