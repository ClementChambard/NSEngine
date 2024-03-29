#include "../NSEngine.h"
#include "AnmVM.h"
#include "AnmManagerN.h"
#include "AnmBitflags.h"
#include "../vertex.h"
#include "../math/Random.h"
#include "../NSlist.h"

namespace NSEngine {

    int AnmVM::cnt = 0;

    void AnmVM::interrupt(int i)
    {
        pending_switch_label = i;
    }

    void AnmVM::interruptRec(int i)
    {
        interrupt(i);
        for (AnmVMList* child = childrens->next; child != nullptr; child = child->next)
        {
            if (child->value) child->value->interruptRec(i);
        }
    }

    AnmVM::AnmVM(AnmVM const& toCopy)
    {
        case_return_time = toCopy.case_return_time ;
        return_instr     = toCopy.return_instr     ;
        layer            = toCopy.layer            ;
        anim_slot        = toCopy.anim_slot        ;
        sprite_id        = toCopy.sprite_id        ;
        script_id        = toCopy.script_id        ;
        current_instr    = toCopy.current_instr    ;
        pos              = toCopy.pos              ;
        rotation         = toCopy.rotation         ;
        angular_velocity = toCopy.angular_velocity ;
        scale            = toCopy.scale            ;
        scale_2          = toCopy.scale_2          ;
        scale_growth     = toCopy.scale_growth     ;
        uv_scale         = toCopy.uv_scale         ;
        uv_scroll_pos    = toCopy.uv_scroll_pos    ;
        anchor_offset    = toCopy.anchor_offset    ;
        uv_scroll_vel_x  = toCopy.uv_scroll_vel_x  ;
        uv_scroll_vel_y  = toCopy.uv_scroll_vel_y  ;
        rot_vars         = toCopy.rot_vars         ;
        script_var_8     = toCopy.script_var_8     ;
        script_var_9     = toCopy.script_var_9     ;
        pos2             = toCopy.pos2             ;
        color1           = toCopy.color1           ;
        alpha1           = toCopy.alpha1           ;
        color2           = toCopy.color2           ;
        alpha2           = toCopy.alpha2.current   ;
        bitflags         = toCopy.bitflags         ;
        rand_scale_1f    = toCopy.rand_scale_1f    ;
        rand_scale_pi    = toCopy.rand_scale_pi    ;
        entity_pos       = toCopy.entity_pos       ;
        on_set_sprite    = toCopy.on_set_sprite    ;
        entity           = toCopy.entity           ;

        for (int i = 0; i < 4; i++) {
            int_vars[i] = toCopy.int_vars[i];
            float_vars[i] = toCopy.float_vars[i];
        }

        num_cycles_in_texture = toCopy.num_cycles_in_texture;
        mode_of_special_draw = toCopy.mode_of_special_draw;
        time = 0;
        pending_switch_label = 0;
        if (childrens != nullptr) delete childrens;
        childrens = new AnmVMList();

        bitflags.activeFlags = ANMVM_ACTIVE;
        bitflags.originMode = 1;
    }

    void AnmVM::operator()(const AnmVM &toCopy)
    {
        case_return_time = toCopy.case_return_time ;
        return_instr     = toCopy.return_instr     ;
        layer            = toCopy.layer            ;
        anim_slot        = toCopy.anim_slot        ;
        sprite_id        = toCopy.sprite_id        ;
        script_id        = toCopy.script_id        ;
        current_instr    = toCopy.current_instr    ;
        pos              = toCopy.pos              ;
        rotation         = toCopy.rotation         ;
        angular_velocity = toCopy.angular_velocity ;
        scale            = toCopy.scale            ;
        scale_2          = toCopy.scale_2          ;
        scale_growth     = toCopy.scale_growth     ;
        uv_scale         = toCopy.uv_scale         ;
        uv_scroll_pos    = toCopy.uv_scroll_pos    ;
        anchor_offset    = toCopy.anchor_offset    ;
        uv_scroll_vel_x  = toCopy.uv_scroll_vel_x  ;
        uv_scroll_vel_y  = toCopy.uv_scroll_vel_y  ;
        rot_vars         = toCopy.rot_vars         ;
        script_var_8     = toCopy.script_var_8     ;
        script_var_9     = toCopy.script_var_9     ;
        pos2             = toCopy.pos2             ;
        color1           = toCopy.color1           ;
        alpha1           = toCopy.alpha1           ;
        color2           = toCopy.color2           ;
        alpha2           = toCopy.alpha2.current   ;
        bitflags         = toCopy.bitflags         ;
        rand_scale_1f    = toCopy.rand_scale_1f    ;
        rand_scale_pi    = toCopy.rand_scale_pi    ;
        entity_pos       = toCopy.entity_pos       ;
        entity           = toCopy.entity           ;
        on_set_sprite    = toCopy.on_set_sprite    ;

        for (int i = 0; i < 4; i++) {
            int_vars[i] = toCopy.int_vars[i];
            float_vars[i] = toCopy.float_vars[i];
        }

        num_cycles_in_texture = toCopy.num_cycles_in_texture;
        mode_of_special_draw = toCopy.mode_of_special_draw;
        time = 0;
        pending_switch_label = 0;

        if (childrens != nullptr) delete childrens;
        childrens = new AnmVMList();

        bitflags.activeFlags = ANMVM_ACTIVE;
        bitflags.originMode = 1;
    }

    AnmVM::AnmVM(uint32_t script_id, uint32_t anim_slot)
    {
        this->anim_slot = anim_slot;
        this->script_id = script_id;

        // When creating via instruction vector, update frame -1 of the VM.
        childrens = new AnmVMList();
        time = -1;
        update();
    }

    enum class bytefields : uint32_t {};
    std::ostream& operator<<(std::ostream& left, bytefields bf)
    {
        for (int i = 31; i >= 0; i--)
            left << (((uint32_t)bf & (1 << i)) != 0);
        return left;
    }

    AnmVM::~AnmVM()
    {
        destroy();
        if (childrens != nullptr) delete childrens;
    }

    AnmSprite AnmVM::getSprite() const { return AnmManagerN::loadedFiles[anim_slot].getSprite(sprite_id); }

    void AnmVM::update(bool printInstr)
    {
        /* VM IS NOT RUNNING */
        if (bitflags.activeFlags != ANMVM_ACTIVE && time != -1) return;
        cnt++;

        /* UPDATE VARIABLES */
        rotation += angular_velocity;
        uv_scroll_pos += glm::vec2{uv_scroll_vel_x.current, uv_scroll_vel_y.current};
        scale += scale_growth;
        pos.update();
        rotation.update();
        scale.update();
        scale_2.update();
        uv_scale.update();
        uv_scroll_vel_x.update();
        uv_scroll_vel_y.update();

        color1.update();
        alpha1.update();
        color2.update();
        alpha2.update();

        int8_t* instructions = AnmManagerN::loadedFiles[anim_slot].getScript(script_id);
        if (instructions == nullptr) return;

        /* CHECK FOR INTERRUPTIONS */
        if (pending_switch_label != 0)
        {
            //find label
            int32_t offset = 0;
            while (instructions[offset] != -1)
            {
                uint16_t instype = *reinterpret_cast<uint16_t*>(&(instructions[offset]));
                uint16_t inslength = *reinterpret_cast<uint16_t*>(&(instructions[offset+2]));
                int16_t instime = *reinterpret_cast<int16_t*>(&(instructions[offset+4]));
                if (instype == 5 && *reinterpret_cast<int32_t*>(&(instructions[offset+8])) == pending_switch_label)
                {
                    return_instr = current_instr;
                    case_return_time = time;
                    current_instr = offset;
                    time = instime;
                    bitflags.visible = 1;
                }
                offset += inslength;
            }
            pending_switch_label = 0;
        }

        /* VM IS STOPPED */
        if (time < -9999) return;

        /* RUN INSTRUCTIONS */
        uint32_t oldinstr = current_instr;
        uint16_t instype, inslength;
        int16_t instime;
        #define getIns if (instructions[current_instr] == -1) return; \
            instype = *reinterpret_cast<uint16_t*>(&(instructions[current_instr])); \
            inslength = *reinterpret_cast<uint16_t*>(&(instructions[current_instr+2])); \
            instime = *reinterpret_cast<int16_t*>(&(instructions[current_instr+4]));
        getIns;
        while(instime <= time && (bitflags.activeFlags == ANMVM_ACTIVE || time == -1))
        {
            exec_instruction(&(instructions[current_instr]));

            if (oldinstr != current_instr)
            {
                getIns;
                if(instime <= time) exec_instruction(&(instructions[current_instr]));
            }
            current_instr+=inslength;
            oldinstr = current_instr;
            getIns;
        }
        time++;
    }
    int AnmVM::getMode() const { return bitflags.rendermode; }
    int AnmVM::getZdis() const { return bitflags.zwritedis || bitflags.blendmode != 0; }

    void AnmVM::destroy()
    {
        bitflags = {};
        if (nodeInGlobalList != nullptr) { delete ListUtil::listRemoveNode(nodeInGlobalList); nodeInGlobalList = nullptr; }
        if (nodeAsChildren   != nullptr) { delete ListUtil::listRemoveNode(  nodeAsChildren); nodeAsChildren   = nullptr; }
        if (childrens != nullptr)
        {
            while (childrens->next) childrens->next->value->destroy();
            delete childrens; childrens = nullptr;
        }

        on_set_sprite = [](AnmVM* me, int spr){ return spr; };
        case_return_time = -99;
        return_instr = 0;
        layer = 0;
        anim_slot = 0;
        sprite_id = 0;
        script_id = 0;
        current_instr = 0;
        pos = {{0,0,0}};
        rotation = {{0,0,0}};
        angular_velocity = {};
        scale = {{1.f,1.f}};
        scale_2 = {{1,1}};
        scale_growth = {};
        uv_scale = {{1, 1}};
        uv_scroll_pos = {};
        anchor_offset = {};
        uv_scroll_vel_x = {};
        uv_scroll_vel_y = {};
        pending_switch_label = 0;
        for (int i = 0; i < 4; i++) {
        int_vars[i] = 0;
        float_vars[i] = 0;}
        rot_vars = {};
        script_var_8 = 0;
        script_var_9 = 0;
        rand_scale_1f = 1.f;
        rand_scale_pi = PI;
        num_cycles_in_texture = 1;
        pos2 = {};
        mode_of_special_draw = -1; // mode_of_create_child
        color1 = {ColorRGB{255, 255, 255}};
        alpha1 = {NSITPuint8_t{255}};
        color2 = {ColorRGB{255, 255, 255}};
        alpha2 = {NSITPuint8_t{255}};
        id = 0;
        fast_id = 0;
        time = 0;
        parent = nullptr;
        entity_pos = {}; // pos3 ?
        entity = nullptr;
    }

}
