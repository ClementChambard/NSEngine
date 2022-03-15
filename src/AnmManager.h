#ifndef ANMMANAGER_H
#define ANMMANAGER_H

#include "AnimScript.h"
#include "GameLayers.h"

class AnmManager {
    public:
        static void Init(uint16_t maxAnim = 10000);
        static void Cleanup() { delete[] anim_pool; }
        static void DeleteAll();

        static void Update(float framespeed, bool gamePaused = false);
        static void Draw();

        static void interrupt(uint16_t i, uint8_t inte);
        static void UpdateChild(uint16_t id, float x, float y, float z, float rx, float ry, float rz, float sx, float sy,
                bool parentNotAnim = false, int interrupt = 0);
        static void chgScript(uint16_t i, void (*anim_script)(AnimScript*), bool firstupdate = false);
        static bool IsActive(uint16_t i);

        static uint16_t newAnim(animscript_t anim_script, uint16_t i = (uint16_t)-1, bool firstupdate = false);
        static uint16_t childAnim(animscript_t anim_script, uint16_t i = (uint16_t)-1);
        static void deleteAnim(uint16_t i);
        static AnimScript* anim(uint16_t i);

        static uint16_t getMaxAnim() { return maxAnim; }
    private:
        static AnimScript* anim_pool;
        static uint16_t maxAnim;
        static uint16_t current_pos;
};

#endif
