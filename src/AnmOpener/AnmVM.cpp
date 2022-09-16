#include <glm/gtx/euler_angles.hpp>
#include "../NSEngine.h"
#include "AnmVM.h"
#include "AnmManagerN.h"
#include "AnmBitflags.h"
#include "../vertex.h"
#include "../math/Random.h"
#include "../NSlist.h"

namespace NSEngine {

    void AnmVM::interrupt(int i)
    {
        pending_switch_label = i;
        for (AnmVMList* child = childrens->next; child != nullptr; child = child->next)
        {
            if (child->value) child->value->interrupt(i);
        }
    }

    AnmVM::AnmVM(AnmVM const& toCopy)
    {
        case_return_time = toCopy.case_return_time ;
        return_instr     = toCopy.return_instr     ;
        layer            = toCopy.layer            ;
        anim_slot        = toCopy.anim_slot        ;
        sprite_id        = toCopy.sprite_id        ;
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
        alpha2           = toCopy.alpha2.current           ;
        bitflags_lo      = toCopy.bitflags_lo      ;
        bitflags_hi      = toCopy.bitflags_hi      ;
        rand_scale_1f    = toCopy.rand_scale_1f    ;
        rand_scale_pi    = toCopy.rand_scale_pi    ;
        entity_pos       = toCopy.entity_pos       ;

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

        ANMVM_SET_ACTIVE(ANMVM_ACTIVE);
        ANMVM_SET_ORIGIN(1);
    }

    void AnmVM::operator()(const AnmVM &toCopy)
    {
        case_return_time = toCopy.case_return_time ;
        return_instr     = toCopy.return_instr     ;
        layer            = toCopy.layer            ;
        anim_slot        = toCopy.anim_slot        ;
        sprite_id        = toCopy.sprite_id        ;
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
        alpha2           = toCopy.alpha2.current           ;
        bitflags_lo      = toCopy.bitflags_lo      ;
        bitflags_hi      = toCopy.bitflags_hi      ;
        rand_scale_1f    = toCopy.rand_scale_1f    ;
        rand_scale_pi    = toCopy.rand_scale_pi    ;
        entity_pos       = toCopy.entity_pos       ;

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

        ANMVM_SET_ACTIVE(ANMVM_ACTIVE);
        ANMVM_SET_ORIGIN(1);
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

    void AnmVM::update(bool printInstr)
    {
        /* VM IS NOT RUNNING */
        if (ANMVM_GET_ACTIVE != ANMVM_ACTIVE && time != -1) return;

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
                    bitflags_lo |= ANMVM_BIT_VISIBLE;
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
        while(instime <= time && (ANMVM_GET_ACTIVE == ANMVM_ACTIVE || time == -1))
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

    void AnmVM::draw()
    {
        /* CHECK IF THE VM IS VISIBLE */
        if (ANMVM_GET_ACTIVE != ANMVM_ACTIVE) return;
        if (!(bitflags_lo & ANMVM_BIT_VISIBLE)) return;
        if (alpha1.current.value == 0 && alpha2.current.value == 0) return;

        /* GET PARENT VARIABLES */
        float px = 0, py = 0, pz = 0, prx = 0, pry = 0, prz = 0, psx = 1, psy = 1;
        if (parent != nullptr)
        {
            px = parent->pos.current.x+parent->entity_pos.x;
            py = -parent->pos.current.y-parent->entity_pos.y;
            pz = parent->pos.current.z+parent->entity_pos.z;
            prx = parent->rotation.current.x;
            pry = parent->rotation.current.y;
            prz = parent->rotation.current.z;
            psx = parent->scale.current.x;
            psy = parent->scale.current.y;
        }

        /* CALCULATE SCALE AND ANCHOR MULTIPLIERS AND COLORS */
        float XS = scale.current.x * scale_2.current.x * psx;
        float YS = scale.current.y * scale_2.current.y * psy;
        float l = ANMVM_GET_ANCHORH != 0 ? -(ANMVM_GET_ANCHORH - 1) : -0.5f;
        float r = l+1;
        float b = ANMVM_GET_ANCHORV != 0 ? -(ANMVM_GET_ANCHORV - 1) : -0.5f;
        float t = b+1;
        Color c1 = Color(color1.current.r,color1.current.g,color1.current.b,alpha1.current);
        Color c2 = Color(color2.current.r,color2.current.g,color2.current.b,alpha2.current);
        uint8_t blendmode = ANMVM_GET_BLEND;

        auto p = pos.current+pos2;
        if (ANMVM_GET_ORIGIN == 0)
        {
            p += glm::vec3(-engineData::gameWidth/2,-engineData::gameHeight/2,0);
        }
        if (!(bitflags_hi & ANMVM_BIT_534_8)) { psx *=2; psy *=2; }

        /* MODE 3 : RECTANGLE      MODE 6 : RECTANGLE GRADIENT      MODE 12 : RECTANGLE BORDER*/
        if (mode_of_special_draw == 6 || mode_of_special_draw == 3 || mode_of_special_draw == 12)
        {
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), rotation.current.z, {0, 0, -1});
            float width = float_vars[2] * XS;
            float height = float_vars[3] * YS;
            glm::vec4 pp = glm::vec4(/*psx */ p.x + px, /*psy */ -p.y + py, p.z + pz, 0);
            glm::vec4 pos1 = pp + rotZ * glm::vec4(- width * l, - height * t, 0, 0);
            glm::vec4 pos2 = pp + rotZ * glm::vec4(+ width * r, - height * t, 0, 0);
            glm::vec4 pos3 = pp + rotZ * glm::vec4(+ width * r, + height * b, 0, 0);
            glm::vec4 pos4 = pp + rotZ * glm::vec4(- width * l, + height * b, 0, 0);
            c2 = mode_of_special_draw == 6 ? c2 : c1;
            draw_quad_color_2d(pos1, pos2, pos3, pos4, c1, c2, c2, c1);
            draw_set_blend(0);
            return;
        }

        /* MODE 4 : POLYGON      MODE 5 : POLYGON OUTLINE */
        if (mode_of_special_draw == 4 || mode_of_special_draw == 5)
        {
            float radius = float_vars[3] * scale.current.x * psx;
            int nbVert = int_vars[3];
            draw_set_layer(layer);
            draw_circle_set_vertex_count(nbVert);
            draw_set_blend(blendmode);
            if (mode_of_special_draw == 5) c2 = c1;
            draw_circle_color(p.x * psx + px, -p.y * psy + py, radius, c1, c2, mode_of_special_draw == 5);
            draw_set_blend(0);
            return;
        }

        /* MODE 13 : LINE */
        if (mode_of_special_draw == 13)
        {
            float len = float_vars[3] * XS;
            glm::mat4 rotate = glm::eulerAngleZYX(-rotation.current.z + prz, rotation.current.y + pry, rotation.current.x + prx);
            glm::vec4 pos2 = glm::vec4(pos.current,0) + rotate * glm::vec4(len, 0, 0, 0);
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            draw_set_color(color1.current.r, color1.current.g, color1.current.b, alpha1.current);
            draw_line(pos.current.x*psx - px, -pos.current.y*psy+py, pos2.x*psx+px, -pos2.y*psy+py, 1);
            draw_set_color(c_white);
            draw_set_blend(0);
            return;
        }

        /* MODE 11 : RING */
        if (mode_of_special_draw == 11)
        {
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            draw_circle_set_vertex_count(int_vars[0]);
            draw_set_color(color1.current.r, color1.current.g, color1.current.b, alpha1.current);
            float r1 = float_vars[3]*XS - float_vars[2]*YS/2.f;
            float r2 = float_vars[3]*XS + float_vars[2]*YS/2.f;
            draw_circle_arc(p.x+px, -p.y+py, r1, r2, 0, PI2);
            draw_set_color(c_white);
            draw_set_blend(0);
            return;
        }

        /* MODE 7 : RECT_ROT    8 : RECT_ROT_GRAD */
        if (mode_of_special_draw == 8 || mode_of_special_draw == 7)
        {
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            float width = float_vars[2] * XS;
            float height = float_vars[3] * YS;
            c2 = mode_of_special_draw == 8 ? c2 : c1;
            glm::mat4 rotate = glm::eulerAngleZYX(-rotation.current.z + prz, rotation.current.y + pry, rotation.current.x + prx);
            // TODO: rotate and fix position
            draw_rectangle_color(p.x+px-width*l,-p.y+py-height*t,p.x+px+width*r,-p.y+py+height*b,c1,c2,c2,c1);
            draw_set_blend(0);
            return;
        }

        /* GET SPRITE DATA */
        auto s = AnmManagerN::loadedFiles[anim_slot].getSprite(sprite_id);
        float u1 = (s.u1+uv_scroll_pos.x)*uv_scale.current.x;
        float u2 = (s.u2+uv_scroll_pos.x)*uv_scale.current.x;
        float v1 = (s.v1+uv_scroll_pos.y)*uv_scale.current.y;
        float v2 = (s.v2+uv_scroll_pos.y)*uv_scale.current.y;

        /* MODE 0,1,2 : ARCS */
        if (mode_of_special_draw == 1 || mode_of_special_draw == 2 || mode_of_special_draw == 0)
        {
            float r1 = scale.current.y-scale.current.x/2.f;
            float r2 = scale.current.y+scale.current.x/2.f;
            float angleStart = -rotation.current.z, angleEnd = angleStart+PI2;
            if (mode_of_special_draw == 1)
            {
                angleStart = -rotation.current.z - rotation.current.x/2.f;
                angleEnd = -rotation.current.z + rotation.current.x/2.f;
            }
            else if (mode_of_special_draw == 2)
            {
                angleEnd = angleStart+rotation.current.x;
            }
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            draw_circle_set_vertex_count(int_vars[0]);
            draw_set_color(color1.current.r, color1.current.g, color1.current.b, alpha1.current);
            draw_circle_arc_textured(p.x*psx+px, -p.y*psy+py, r1, r2, angleStart, angleEnd, s.texID, u1, u2, int_vars[1]);
            draw_set_color(c_white);
            draw_set_blend(0);
            return;
        }

        /* MODE 10 : RING 3D */
        if (mode_of_special_draw == 10)
        {
            float r1 = float_vars[2] - float_vars[1]/2.f;
            float r2 = float_vars[2] + float_vars[1]/2.f;
            float a1 = float_vars[3] - float_vars[0]/2.f;
            float a2 = float_vars[3] + float_vars[0]/2.f;
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            draw_circle_set_vertex_count(int_vars[0]);
            draw_set_color(color1.current.r, color1.current.g, color1.current.b, alpha1.current);
            draw_circle_arc_textured(p.x+px, -p.y+py, r1, r2, a1, a2, s.texID, u1, u2, int_vars[1]);
            draw_set_color(c_white);
            draw_set_blend(0);
            return;
        }

        /* MODE 9 : CYLINDER */
        if (mode_of_special_draw == 9)
        {
            float r = float_vars[2];
            float h = float_vars[1];
            float a1 = float_vars[3] - float_vars[0]/2.f;
            float a2 = float_vars[3] + float_vars[0]/2.f;
            glm::vec3 posi = {p.x+px, -p.y+py, p.z+pz};
            glm::vec3 rota = rotation.current + glm::vec3(prx, pry, prz);
            draw_set_layer(layer);
            draw_set_blend(blendmode);
            draw_circle_set_vertex_count(int_vars[0]);
            draw_set_color(color1.current.r, color1.current.g, color1.current.b, alpha1.current);
            draw_cylinder(posi, rota, r, h, a1, a2, s.texID, u1, u2, int_vars[1]);
            draw_set_color(c_white);
            draw_set_blend(0);
            return;
        }

        /* CALCULATE POSITION VECTORS */
        //glm::mat4 parentRotate = glm::eulerAngleYXZ(pry, prx, prz);
        //glm::vec4 pos4 = parentRotate * glm::vec4(p,0);
        glm::vec4 pos4 = glm::vec4(p,0);
        pos4.x *= psx;
        pos4.y *= -psy;
        glm::mat4 rotate = glm::eulerAngleZYX(rotation.current.z - prz, rotation.current.y + pry, -rotation.current.x - prx);

        pos4 = glm::vec4(pos4.x+px+entity_pos.x,pos4.y+py-entity_pos.y,pos4.z+pz+entity_pos.z,0);
 /*       if (layer == 3) {
            pos4.y += 2*224.f;
            XS*=2;
            YS*=2;
        } */
        if (bitflags_lo & ANMVM_BIT_ZWRITDI) pos4.z = 0;

        uint8_t colmod = ANMVM_GET_COLMO;
        if (colmod == 1) c1 = c2;
        Color ctl = c1;
        Color ctr = c1;
        Color cbr = c1;
        Color cbl = c1;
        if (colmod == 2) ctr = cbr = c2;
        if (colmod == 3) cbl = cbr = c2;

        /* DRAW ON LAYER */
        Vertex tl = {{pos4 + rotate * glm::vec4(l * s.w * XS + anchor_offset.x, t * s.h * YS + anchor_offset.y, 0, 0)}, ctl, {u1, v1}};
        Vertex tr = {{pos4 + rotate * glm::vec4(r * s.w * XS + anchor_offset.x, t * s.h * YS + anchor_offset.y, 0, 0)}, ctr, {u2, v1}};
        Vertex br = {{pos4 + rotate * glm::vec4(r * s.w * XS + anchor_offset.x, b * s.h * YS + anchor_offset.y, 0, 0)}, cbr, {u2, v2}};
        Vertex bl = {{pos4 + rotate * glm::vec4(l * s.w * XS + anchor_offset.x, b * s.h * YS + anchor_offset.y, 0, 0)}, cbl, {u1, v2}};

        engineData::layers[layer]->getBatch()->draw(s.texID, tl, tr, br, bl, blendmode);

    }

    int& AnmVM::check_ref(int i)
    {
        if      (i >= 10000 && i < 10004) return int_vars[static_cast<size_t>(i-10000)];
        else if (i == 10008) return script_var_8;
        else if (i == 10009) return script_var_9;
        //else if (i == 10029) return 65535;
        else return num_cycles_in_texture;
    }

    float& AnmVM::check_ref(float f)
    {
        if      (f >= 10004 && f < 10008) return float_vars[static_cast<size_t>(f-10004)];
        else if (f == 10013.f) return pos.current.x;
        else if (f == 10014.f) return pos.current.y;
        else if (f == 10015.f) return pos.current.z;
        else if (f == 10023.f) return rotation.current.x;
        else if (f == 10024.f) return rotation.current.y;
        else if (f == 10025.f) return rotation.current.z;
        else if (f == 10027.f) return rand_scale_1f;
        else if (f == 10028.f) return rand_scale_pi;
        else if (f == 10033.f) return rot_vars.x;
        else if (f == 10034.f) return rot_vars.y;
        else if (f == 10035.f) return rot_vars.z;
        else return float_vars[3];
    }

    int AnmVM::check_val(int i)
    {
        if      (i >= 10000 && i < 10004) return int_vars[static_cast<size_t>(i-10000)];
        else if (i == 10008) return script_var_8;
        else if (i == 10009) return script_var_9;
        else if (i == 10022) return static_cast<int>(Random::Float01() * 65535);
        else if (i == 10029) return 65535;
        else return i;
    }

    float AnmVM::check_val(float f)
    {
        if      (f >= 10004.f && f <  10008.f) return float_vars[static_cast<size_t>(f-10004.f)];
        else if (f == 10010.f || f == 10030.f) return Random::Floatm11() * rand_scale_pi;
        else if (f == 10011.f || f == 10031.f) return Random::Float01() * rand_scale_1f;
        else if (f == 10012.f || f == 10032.f) return Random::Floatm11() * rand_scale_1f;
        else if (f == 10013.f) return pos.current.x;
        else if (f == 10014.f) return pos.current.y;
        else if (f == 10015.f) return pos.current.z;
        else if (f == 10016.f) return (activeCamera3D() ? activeCamera3D()->getPosition().x : 0.f);
        else if (f == 10017.f) return (activeCamera3D() ? activeCamera3D()->getPosition().y : 0.f);
        else if (f == 10018.f) return (activeCamera3D() ? activeCamera3D()->getPosition().z : 0.f);
        else if (f == 10019.f) return (activeCamera3D() ? activeCamera3D()->getLook().x : 0.f);
        else if (f == 10020.f) return (activeCamera3D() ? activeCamera3D()->getLook().y : 0.f);
        else if (f == 10021.f) return (activeCamera3D() ? activeCamera3D()->getLook().z : 1.f);
        else if (f == 10023.f) return rotation.current.x;
        else if (f == 10024.f) return rotation.current.y;
        else if (f == 10025.f) return rotation.current.z;
        else if (f == 10026.f) return rotation.current.z + (parent ? parent->rotation.current.z : 0);
        else if (f == 10027.f) return rand_scale_1f;
        else if (f == 10028.f) return rand_scale_pi;
        else if (f == 10033.f) return rot_vars.x;
        else if (f == 10034.f) return rot_vars.y;
        else if (f == 10035.f) return rot_vars.z;
        else return f;
    }

    void AnmVM::destroy()
    {
        bitflags_lo=0;
        bitflags_hi=0;
        if (nodeInGlobalList != nullptr) { delete ListUtil::listRemoveNode(nodeInGlobalList); nodeInGlobalList = nullptr; }
        if (nodeAsChildren   != nullptr) { delete ListUtil::listRemoveNode(  nodeAsChildren); nodeAsChildren   = nullptr; }
        if (childrens != nullptr)
        {
            while (childrens->next) childrens->next->value->destroy();
            delete childrens; childrens = nullptr;
        }
    }

    void AnmVM::exec_instruction(int8_t* ins)
    {
        float tempvar1 = 0.f;
        float tempvar2 = 0.f;
        AnmVMList* tempNode = nullptr;
        uint16_t type = *reinterpret_cast<uint16_t*>(ins);
        #define argS(x) *reinterpret_cast<int32_t*>(&(ins[8+4*x]))
        #define argf(x) *reinterpret_cast<float*>(&(ins[8+4*x]))
        #define  S(x) check_val(argS(x))
        #define u8S(x) static_cast<uint8_t>(S(x))
        #define  f(x) check_val(argf(x))
        #define rS(x) check_ref(argS(x))
        #define rf(x) check_ref(argf(x))
        #define flagl(f, b) if (b              )  bitflags_lo |=  f; else bitflags_lo &= ~f;
        #define flagTl(f)   if (bitflags_lo & f) bitflags_lo &= ~f; else bitflags_lo |=  f;
        #define flagh(f, b) if (b              ) bitflags_hi |=  f; else bitflags_hi &= ~f;
        #define flagTb(f)   if (bitflags_hi & f) bitflags_hi &= ~f; else bitflags_hi |=  f;
        #define jump(n, t) {current_instr = n; time = t;}
        #define createChild(id, u, l)     \
                        tempNode = new AnmVMList();     \
                        tempNode->value = AnmManagerN::getVM(AnmManagerN::SpawnVM(anim_slot, id, u, l));  \
                        tempNode->value->parent = this;                         \
                        tempNode->value->nodeAsChildren = tempNode
        switch (type)
        {

            case   0: // nop
                return;
            case   1: // destroy
                ANMVM_SET_ACTIVE(ANMVM_DELETE);
                return;
            case   2: // freeze
                ANMVM_SET_ACTIVE(ANMVM_FROZEN);
                return;
            case   3: // stop
                if (time >= -10000) time = -10000-time;
                return;
            case   4: // stophide
                flagl(ANMVM_BIT_VISIBLE, 0);
                if (time >= -10000) time = -10000-time;
                return;
            case   5: // interruptLabel
                // do nothing (just a label)
                return;
            case   6: // wait
                time -= S(0);
                return;
            case   7: // caseReturn
                time = case_return_time;
                current_instr = return_instr;
                return;

            case 100: // iset
                rS(0)  = S(1);
                return;
            case 101: // fset
                rf(0)  = f(1);
                return;
            case 102: // iadd
                rS(0) += S(1);
                return;
            case 103: // fadd
                rf(0) += f(1);
                return;
            case 104: // isub
                rS(0) -= S(1);
                return;
            case 105: // fsub
                rf(0) -= f(1);
                return;
            case 106: // imul
                rS(0) *= S(1);
                return;
            case 107: // fmul
                rf(0) *= f(1);
                return;
            case 108: // idiv
                rS(0) /= S(1);
                return;
            case 109: // fdiv
                rf(0) /= f(1);
                return;
            case 110: // imod
                rS(0) %= S(1);
                return;
            case 111: // fmod
                rf(0) = fmod(f(0), f(1));
                return;
            case 112: // isetadd
                rS(0) = S(1) + S(2);
                return;
            case 113: // fsetadd
                rf(0) = f(1) + f(2);
                return;
            case 114: // isetsub
                rS(0) = S(1) - S(2);
                return;
            case 115: // fsetsub
                rf(0) = f(1) - f(2);
                return;
            case 116: // isetmul
                rS(0) = S(1) * S(2);
                return;
            case 117: // fsetmul
                rf(0) = f(1) * f(2);
                return;
            case 118: // isetdiv
                rS(0) = S(1) / S(2);
                return;
            case 119: // fsetdiv
                rf(0) = f(1) / f(2);
                return;
            case 120: // isetmod
                rS(0) = S(1) % S(2);
                return;
            case 121: // fsetmod
                rf(0) = fmod(f(1), f(2));
                return;
            case 122: // isetrand
                rS(0) = Random::Float01() * S(1);
                return;
            case 123: // fsetrand
                rf(0) = Random::Float01() * f(1);
                return;
            case 124: // fsin
                rf(0) = sin(f(1));
                return;
            case 125: // fcos
                rf(0) = cos(f(1));
                return;
            case 126: // ftan
                rf(0) = tan(f(1));
                return;
            case 127: // facos
                rf(0) = acos(f(1));
                return;
            case 128: // fatan
                rf(0) = atan(f(1));
                return;
            case 129: // validAngle
                while (f(0) > PI) rf(0) -= PI2;
                while (f(0) < -PI) rf(0) += PI2;
                return;
            case 130: // circlePos
                rf(0) = f(3) * cos(f(2));
                rf(1) = f(3) * sin(f(2));
                return;
            case 131: // circlePosRand
                tempvar1 = f(2) + (f(3)-f(2)) * Random::Float01();
                tempvar2 = Random::Angle();
                rf(0) = tempvar1 * cos(tempvar2);
                rf(1) = tempvar1 * sin(tempvar2);
                return;

            case 200: // jmp
                jump(S(0), S(1))
                return;
            case 201: // jmpDec
                if (rS(0)-- > 0) jump(S(1), S(2))
                return;
            case 202: // ije
                if (S(0) == S(1)) jump(S(2), S(3))
                return;
            case 203: // fje
                if (f(0) == f(1)) jump(S(2), S(3))
                return;
            case 204: // ijne
                if (S(0) != S(1)) jump(S(2), S(3))
                return;
            case 205: // fjne
                if (f(0) != f(1)) jump(S(2), S(3))
                return;
            case 206: // ijl
                if (S(0) <  S(1)) jump(S(2), S(3))
                return;
            case 207: // fjl
                if (f(0) <  f(1)) jump(S(2), S(3))
                return;
            case 208: // ijle
                if (S(0) <= S(1)) jump(S(2), S(3))
                return;
            case 209: // fjle
                if (f(0) <= f(1)) jump(S(2), S(3))
                return;
            case 210: // ijg
                if (S(0) >  S(1)) jump(S(2), S(3))
                return;
            case 211: // fjg
                if (f(0) >  f(1)) jump(S(2), S(3))
                return;
            case 212: // ijge
                if (S(0) >= S(1)) jump(S(2), S(3))
                return;
            case 213: // fjge
                if (f(0) >= f(1)) jump(S(2), S(3))
                return;

            case 300: // sprite
                sprite_id = S(0);
                flagl(ANMVM_BIT_VISIBLE, 1);
                return;
            case 301: // spriteRand
                tempvar1 = S(0) + (S(1)-S(0)) * Random::Float01();
                sprite_id = static_cast<uint32_t>(tempvar1);
                return;
            case 302: // type
                ANMVM_SET_MODE(S(0));
                return;
            case 303: // blendmode
                ANMVM_SET_BLEND(S(0));
                return;
            case 304: // layer
                layer = S(0);
                return;
            case 305: // zWriteDisable
                flagl(ANMVM_BIT_ZWRITDI, S(0));
                return;
            case 306: // ins_306 : std related bitflag
                flagl(ANMVM_BIT_530_15, S(0));
                return;
            case 307: // randMode
                // do nothing : only one type of random
                return;
            case 308: // flipX
                scale.current.x *= -1;
                scale.goal.x *= -1;
                flagTl(ANMVM_BIT_FLIP_X);
                return;
            case 309: // flipY
                scale.current.y *= -1;
                scale.goal.y *= -1;
                flagTl(ANMVM_BIT_FLIP_Y);
                return;
            case 310: // visible
                flagl(ANMVM_BIT_VISIBLE, S(0))
                return;
            case 311: // resampleMode
                ANMVM_SET_RESMOD(S(0));
                return;
            case 312: // scrollMode
                ANMVM_SET_SCROLX(S(0));
                ANMVM_SET_SCROLY(S(1));
                return;
            case 313: // resolutionMode
                ANMVM_SET_RESMOD(S(0));
                return;
            case 314: // ins_314
                flagh(ANMVM_BIT_PAR_ROT, S(0));
                return;
            case 315: // colorizeChildrens
                flagh(ANMVM_BIT_COL_CHI, S(0));
                return;
            case 316: // ins_316
                flagl(ANMVM_BIT_530_1, 1);
                return;
            case 317: // ins_317
                flagl(ANMVM_BIT_530_1, 0);
                return;

            case 400: // pos
                pos = { f(0), f(1), f(2) };
                return;
            case 401: // rotate
                rotation = { f(0), f(1), f(2) };
                return;
            case 402: // scale
                scale = { f(0), f(1) };
                return;
            case 403: // alpha
                alpha1.current = u8S(0);
                return;
            case 404: // color
                color1 = { u8S(0), u8S(1), u8S(2) };
                return;
            case 405: // alpha2.current
                alpha2.current = u8S(0);
                return;
            case 406: // color2
                color2 = { u8S(0), u8S(1), u8S(2) };
                return;
            case 407: // posTime
                pos.start(pos.current, { f(2), f(3), f(4) }, S(1), S(0));
                return;
            case 408: // colorTime
                color1.start(color1.current, { u8S(2), u8S(3), u8S(4) }, S(1), S(0));
                return;
            case 409: // alphaTime
                alpha1.start(alpha1.current, (NSITPuint8_t)u8S(2), S(1), S(0));
                return;
            case 410: // rotateTime
                rotation.start(rotation.current, { f(2), f(3), f(4) }, S(1), S(0));
                return;
            case 411: // rotateTime2D
                rotation.start(rotation.current, { rotation.current.x, rotation.current.y, f(2) }, S(1), S(0));
                return;
            case 412: // scaleTime
                scale.start(scale.current, { f(2), f(3) }, S(1), S(0));
                return;
            case 413: // color2Time
                color2.start(color2.current, { u8S(2), u8S(3), u8S(4) }, S(1), S(0));
                return;
            case 414: // alpha2.currentTime
                alpha2.start(alpha2.current, (NSITPuint8_t)u8S(2), S(1), S(0));
                return;
            case 415: // rotVel
                angular_velocity = { f(0), f(1), f(2) };
                return;
            case 416: // scaleGrowth
                scale_growth = { f(0), f(1) };
                return;
            case 417: // alphaTimeLinear
                alpha1.start(alpha1.current, (NSITPuint8_t)u8S(1), 0, S(0));
                return;
            case 418: // TODO: ins_418
                // get some uvs from position
                return;
            case 419: // ins_419
                flagh(ANMVM_BIT_534_13, S(0));
                return;
            case 420: // moveBezier
                pos.start(pos.current, {f(4), f(5), f(6)}, 80, S(0), {f(1), f(2), f(3)}, {f(7), f(8), f(9)});
                return;
            case 421: // anchor
                ANMVM_SET_ANCHORH(S(0) & 3);
                ANMVM_SET_ANCHORV((S(0) << 16) & 3);
                return;
            case 422: // ins_422
                pos = entity_pos;
                entity_pos = {0, 0, 0};
                return;
            case 423: // colorMode
                ANMVM_SET_COLMO(S(0));
            case 424: // rotateAuto
                flagh(ANMVM_BIT_AUTOROT, S(0));
                return;
            case 425: // scrollX
                uv_scroll_vel_x.current = f(0);
                return;
            case 426: // scrollY
                uv_scroll_vel_y.current = f(0);
                return;
            case 427: // scrollXTime
                uv_scroll_vel_x.start(uv_scroll_vel_x.current, f(2), S(1), S(0));
                return;
            case 428: // scrollYTime
                uv_scroll_vel_y.start(uv_scroll_vel_y.current, f(2), S(1), S(0));
                return;
            case 429: // zoomOut
                uv_scale = { f(0), f(1) };
                return;
            case 430: // zoomOutTime
                uv_scale.start(uv_scale.current, { f(2), f(3) }, S(1), S(0));
                return;
            case 431: // ins_431
                flagh(ANMVM_BIT_534_8, S(0));
                return;
            case 432: // ins_432
                flagh(ANMVM_BIT_534_9, S(0));
                return;
            case 433: // ins_433
                pos.start(pos.current, { f(3)*cos(f(2)), f(3)*sin(f(2)), pos.current.z }, S(1), S(0));
                return;
            case 434: // scale2
                scale_2 = { f(0), f(1) };
                return;
            case 435: // scale2Time
                scale_2.start(scale_2.current, { f(2), f(3) }, S(1), S(0));
                return;
            case 436: // anchorOffset
                anchor_offset = { -f(0), f(1) };
                return;
            case 437: // rotationMode
                ANMVM_SET_ROTMOD(S(0));
                return;
            case 438: // originMode
                ANMVM_SET_ORIGIN(S(0));
                return;
            case 439: // TODO: fadeNearCamera
                return;

            case 500: // scriptNew
                createChild(S(0), false, false);
                ListUtil::listInsertAfter(childrens, tempNode);
                // TODO: childs copy some bitflags
                return;
            case 501: // scriptNewUI
                createChild(S(0), true, false);
                ListUtil::listInsertAfter(childrens, tempNode);
                return;
            case 502: // scriptNewFront
                createChild(S(0), false, true);
                ListUtil::listInsertAfter(childrens, tempNode);
                tempNode->value->update();
                return;
            case 503: // scriptNewFrontUI
                createChild(S(0), true, true);
                ListUtil::listInsertAfter(childrens, tempNode);
                tempNode->value->update();
                return;
            case 504: // scriptNewRoot
                AnmManagerN::SpawnVM(anim_slot, S(0));
                return;
            case 505: // scriptNewPos
                createChild(S(0), false, false);
                ListUtil::listInsertAfter(childrens, tempNode);
                childrens->next->value->pos2 = { f(1), f(2), 0.f };
                return;
            case 506: // scriptNewPosRoot
                tempNode = new AnmVMList();
                tempNode->value = AnmManagerN::getVM(AnmManagerN::SpawnVM(anim_slot, S(0)));
                tempNode->value->pos2 = { f(1), f(2), 0.f };
                delete tempNode;
                return;
            case 507: // ins_507 (ignore parent)
                flagh(ANMVM_BIT_NOPAREN, S(0));
                return;
            case 508: // TODO: effectNew
                std::cout << "[WARNING] anm: instruction 508 (effectNew) is not implemented\n";
                return;
            case 509: // copyVars
                if (parent == nullptr) return;
                for (unsigned int i = 0; i < 4; i++)
                {
                    int_vars[i] = parent->int_vars[i];
                    float_vars[i] = parent->float_vars[i];
                }
                rot_vars = parent->rot_vars;
                script_var_8 = parent->script_var_8;
                script_var_9 = parent->script_var_9;
                rand_scale_1f = parent->rand_scale_1f;
                rand_scale_pi = parent->rand_scale_pi;
                return;

            case 600: // texCircle
                mode_of_special_draw = 0;
                return;
            case 601: // texArcEven
                mode_of_special_draw = 1;
                return;
            case 602: // texArc
                mode_of_special_draw = 2;
                return;
            case 603: // drawRect
                mode_of_special_draw = 3;
                float_vars[2] = f(0);
                float_vars[3] = f(1);
                return;
            case 604: // drawPoly
                mode_of_special_draw = 4;
                float_vars[3] = f(0);
                int_vars[3] = S(1);
                return;
            case 605: // drawPolyBorder
                mode_of_special_draw = 5;
                float_vars[3] = f(0);
                int_vars[3] = S(1);
                return;
            case 606: // drawRectGrad
                mode_of_special_draw = 6;
                float_vars[2] = f(0);
                float_vars[3] = f(1);
                return;
            case 607: // drawRectRot
                mode_of_special_draw = 7;
                float_vars[2] = f(0);
                float_vars[3] = f(1);
                return;
            case 608: // drawRectRotGrad
                mode_of_special_draw = 8;
                float_vars[2] = f(0);
                float_vars[3] = f(1);
                return;
            case 609: // texCylinder3D
                mode_of_special_draw = 9;
                return;
            case 610: // texRing3D
                mode_of_special_draw = 10;
                return;
            case 611: // drawRing
                mode_of_special_draw = 11;
                float_vars[3] = f(0);
                float_vars[2] = f(1);
                int_vars[3] = S(2);
                return;
            case 612: // drawRectBorder
                mode_of_special_draw = 12;
                float_vars[2] = f(0);
                float_vars[3] = f(1);
                return;
            case 613: // drawLine
                mode_of_special_draw = 13;
                float_vars[3] = f(0);
                return;
            case 614: // ins_614
                mode_of_special_draw = 14;
                float_vars[3] = f(0);
                float_vars[2] = f(1);
                return;

        }
        #undef u8S
        #undef S
        #undef f
        #undef rS
        #undef rf
        #undef argf
        #undef argS
        #undef flagl
        #undef flagTl
        #undef flagh
        #undef flagTh
        #undef jump
        #undef createChild
    }

}
