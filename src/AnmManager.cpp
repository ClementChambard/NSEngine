#include "AnmManager.h"
#ifdef HAS_TEXT_ANM
#include "../textAnm.h"
#endif

AnimScript* AnmManager::anim_pool;
uint16_t AnmManager::current_pos;
uint16_t AnmManager::maxAnim;

void AnmManager::Init(uint16_t max)
{
    maxAnim = max;
    anim_pool = new AnimScript[maxAnim];
    for (uint16_t i = 0; i < maxAnim; i++) anim_pool[i].destroy();
    current_pos = 0;
}
        
void AnmManager::DeleteAll()
{
    for (uint16_t i = 0; i < maxAnim; i++) anim_pool[i].destroy();
}

void AnmManager::Update(float framespeed, bool gamePaused)
{
#ifdef HAS_TEXT_ANM
    PlayerHUD::anmscr = 0;
#endif
    for (uint16_t i = 0; i < maxAnim; i++) 
    {
        if (!(anim_pool[i].flags & ANM_FLAG_ACTIVE)) continue;
#ifdef HAS_TEXT_ANM
        PlayerHUD::anmscr ++;
#endif
        if (gamePaused && !(anim_pool[i].flags & ANM_FLAG_GUI)) continue;
        if (!anim_pool[i].has_parent) anim_pool[i].Update(1.f);
    }
}
        
void AnmManager::interrupt(uint16_t i, uint8_t inte)
{
    anim_pool[i].raise_interrupt(inte);
}  

void AnmManager::UpdateChild(uint16_t id, float x, float y, float z, float rx, float ry, float rz, float xs, float ys, 
        bool parentNotAnim, int interrupt)
{
    if (id >= maxAnim) return;
    if (anim_pool[id].has_parent == true || parentNotAnim)
    {
        if (anim_pool[id].interrupt == 0)
            anim_pool[id].interrupt = interrupt;
        anim_pool[id].set_parent_data(x,y,z,rx,ry,rz,xs,ys);
        if (!parentNotAnim)
            anim_pool[id].Update(1);
    }
}

void AnmManager::Draw()
{
    for (int i = 0; i < maxAnim; i++) anim_pool[i].Draw();
#ifdef HAS_TEXT_ANM
    AsciiManager::Draw();
#endif
}

void AnmManager::chgScript(uint16_t i, void (*anim_script)(AnimScript*), bool firstupdate)
{
    if (i >= maxAnim) return;
    newAnim(anim_script,i,firstupdate);
}

bool AnmManager::IsActive(uint16_t i)
{
    if (i >= maxAnim) return false;
    return anim_pool[i].flags & ANM_FLAG_ACTIVE;
}

void AnmManager::deleteAnim(uint16_t i)
{
    if (i >= maxAnim) return;
    anim_pool[i].destroy();
}

AnimScript* AnmManager::anim(uint16_t i)
{
    if (i >= maxAnim) return nullptr;
    return &(anim_pool[i]);
}

uint16_t AnmManager::newAnim(animscript_t anim_script, uint16_t i, bool firstupdate)
{
    if (i < maxAnim)
    {
        anim_pool[i].Init(anim_script);
        if (firstupdate) anim_pool[i].Update(1);
        return i;
    }
    i = 0;
    while(anim_pool[current_pos].flags&ANM_FLAG_ACTIVE && i++ < maxAnim) { current_pos = (current_pos + 1) % maxAnim;}
    anim_pool[current_pos].Init(anim_script);
    i = current_pos;
    current_pos = (current_pos + 1) % maxAnim;
    if (firstupdate) anim_pool[i].Update(1);
    return i;
}

uint16_t AnmManager::childAnim(void (*anim_script)(AnimScript*), uint16_t i)
{
    if (i < maxAnim)
    {
        anim_pool[i].Init(anim_script);
        anim_pool[i].has_parent = true;
        return i;
    }
    i = 0;
    while(anim_pool[current_pos].flags&ANM_FLAG_ACTIVE && i++ < maxAnim) { current_pos = (current_pos + 1) % maxAnim;}
    anim_pool[current_pos].Init(anim_script);
    anim_pool[current_pos].has_parent = true;
    i = current_pos;
    current_pos = (current_pos + 1) % maxAnim;
    return i;
}

