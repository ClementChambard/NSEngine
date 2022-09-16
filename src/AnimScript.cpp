#include "AnimScript.h"
#include "GameLayers.h"
#include "AnmManager.h"
#include "NSEngine.h"
#include "Interpolator.h"
#include <glm/gtx/euler_angles.hpp>

AnimScript::AnimScript() 
{
    Init(nullptr);
}

void AnimScript::Init(animscript_t func_update)
{
    destroy();
    this->func_update = func_update;
    flags = ANM_FLAG_ACTIVE|ANM_FLAG_VISIBLE;
}

void AnimScript::SetScript(animscript_t func_update)
{
    this->func_update = func_update;
}

void AnimScript::destroy()
{
    specialdraw = 0; 
    flags = 0;
    for (int id : childs_id) AnmManager::deleteAnim(id); 
    childs_id.clear(); 
    has_parent = false; 
    set_parent_data(0,0,0,0,0,0,1,1); 
    frame = 0; 
    sprit = -1; 
    lyr = -1; 
    interrupt = 0; 
    pos = glm::vec3(0.f, 0.f, 0.f); 
    u = 0.f;
    v = 0.f;
    scale = glm::vec2(1.f, 1.f);
    uvz = glm::vec2(1.f, 1.f);
    rot = glm::vec3(0.f, 0.f, 0.f);
    col1 = NSEngine::ColorRGB(255, 255, 255);
    col2 = NSEngine::ColorRGB(255, 255, 255);
    a = 255;
    a2 = 255;
    rx_vel = 0;
    ry_vel = 0;
    rz_vel = 0;
    sx_vel = 0;
    sy_vel = 0;
    u_vel = 0;
    v_vel = 0;
    su_vel = 0;
    sv_vel = 0;
    blendmode=0;
}

uint16_t AnimScript::create_child(animscript_t func_update, bool firstupdate)
{
    uint16_t a;
    if (childs_id.size() < 50) 
        childs_id.push_back(a = AnmManager::childAnim(func_update));
    else 
        for (uint8_t i = 0; i < 50; i++) 
            if (!AnmManager::IsActive(childs_id[i])) 
            {
                childs_id[i] = a = AnmManager::childAnim(func_update);
                break;
            }
    if (firstupdate)
        AnmManager::anim(a)->Update(1);
    return a;
}

void AnimScript::Update(float framespeed)
{
    if (!(flags&ANM_FLAG_ACTIVE)) return;
    if (func_update != nullptr)
        func_update(this);
    u += u_vel;
    v += v_vel;
    scale.x += sx_vel;
    scale.y += sy_vel;
    uvz.x += su_vel;
    uvz.y += sv_vel;
    rot += glm::vec3(rx_vel, ry_vel, rz_vel);
    for (int id : childs_id) 
    {
        AnmManager::UpdateChild(id,pos.x+px,pos.y+py,pos.z+pz,rot.x+prx,rot.y+pry,rot.z+prz,scale.x*psx,scale.y*psy,false,interrupt);
        if (flags & ANM_FLAG_COLCHILD) { AnmManager::anim(id)->set_color(col1.r,col1.g,col1.b); AnmManager::anim(id)->set_alpha(a); }
    }
    interrupt = 0;
    frame++;
}

bool AnimScript::isNotDrawable()
{
    return (!(flags&ANM_FLAG_ACTIVE)||!(flags&ANM_FLAG_VISIBLE)||lyr==0||sprit==0||a==0.f);
}

void AnimScript::Draw()
{
    if (isNotDrawable()) return;
    float XS = flags & ANM_FLAG_FLIP_X ? -(scale.x * psx) : scale.x * psx;
    float YS = flags & ANM_FLAG_FLIP_Y ? -(scale.y * psy) : scale.y * psy;
    if (specialdraw == 6)
    {
        NSEngine::draw_set_layer(lyr - 1);
        NSEngine::draw_set_blend(blendmode);
        NSEngine::Color c1 = NSEngine::Color(col1.r,col1.g,col1.b,a);
        NSEngine::Color c2 = NSEngine::Color(col2.r,col2.g,col2.b,a2);
        NSEngine::draw_rectangle_color(pos.x+px-XS/2.f,pos.y+py-YS/2.f,pos.x+px+XS/2.f,pos.y+py+YS/2.f,c1,c2,c2,c1);
        NSEngine::draw_set_blend(0);
        return;
    }
    if (specialdraw == 4 || specialdraw == 5)
    {
        NSEngine::draw_set_layer(lyr - 1);
        NSEngine::draw_circle_set_vertex_count((int) scale.y);
        NSEngine::draw_set_blend(blendmode);
        NSEngine::Color c1 = NSEngine::Color(col1.r,col1.g,col1.b,a);
        NSEngine::Color c2 = NSEngine::Color(col2.r,col2.g,col2.b,a2);
        NSEngine::draw_circle_color(pos.x+px,pos.y+py,XS,c1,c2,specialdraw==5);
        NSEngine::draw_set_blend(0);
        return;
    }
    sprite_data* s = sprite(sprit - 1);
    if (specialdraw == 1)
    {
        NSEngine::draw_set_layer(lyr - 1);
        NSEngine::draw_set_blend(blendmode);
        NSEngine::draw_circle_set_vertex_count( (int) pos.z);
        NSEngine::draw_set_color(col1.r, col1.g, col1.b, a);
        NSEngine::draw_circle_arc_textured(pos.x+px,pos.y+py,scale.y-scale.x/2.f,scale.y+scale.x/2.f,rot.z,rot.z+rot.y,s->tex_id,s->u1,s->u2,(int) rot.x);
        NSEngine::draw_set_color(c_white);
        NSEngine::draw_set_blend(0);
        return;
    }

    glm::mat4 parentRotate = glm::eulerAngleYXZ(pry, prx, prz);
    glm::vec4 pos4 = parentRotate * glm::vec4(pos,0); 
    pos4.x *= psx;
    pos4.y *= psy;
    glm::mat4 rotate = glm::eulerAngleYXZ(rot.y + pry, rot.x + prx, rot.z + prz);
    NSEngine::Color c = NSEngine::Color(col1.r,col1.g,col1.b,a);
    float l, r, t, b; 
    if (flag(ANM_FLAG_OFFL)) { l = 0; r = 1; }
    else if (flag(ANM_FLAG_OFFR)) { l = 1; r = 0;}
    else { l = -0.5f; r = 0.5f; }
    if (flag(ANM_FLAG_OFFT)) { t = 0; b = 1; }
    else if (flag(ANM_FLAG_OFFB)) { t = 1; b = 0;}
    else { b = -0.5f; t = 0.5f; }
    if (s == nullptr) return;
    pos4 = glm::vec4(pos4.x+px,pos4.y+py,pos4.z+pz,0);
    if (flags & ANM_FLAG_DISABLE_3D) pos.z = 0;
    NSEngine::Vertex tl = {NSEngine::posfromvec(pos4 + rotate * glm::vec4(l*s->w*XS+ox,t*s->h*YS+oy,0,0)),c,{(s->u1+u)*uvz.x,(s->v1+v)*uvz.y}};
    NSEngine::Vertex tr = {NSEngine::posfromvec(pos4 + rotate * glm::vec4(r*s->w*XS+ox,t*s->h*YS+oy,0,0)),c,{(s->u2+u)*uvz.x,(s->v1+v)*uvz.y}};
    NSEngine::Vertex br = {NSEngine::posfromvec(pos4 + rotate * glm::vec4(r*s->w*XS+ox,b*s->h*YS+oy,0,0)),c,{(s->u2+u)*uvz.x,(s->v2+v)*uvz.y}};
    NSEngine::Vertex bl = {NSEngine::posfromvec(pos4 + rotate * glm::vec4(l*s->w*XS+ox,b*s->h*YS+oy,0,0)),c,{(s->u1+u)*uvz.x,(s->v2+v)*uvz.y}};
    layer(lyr - 1)->getBatch()->draw(s->tex_id, tl, tr, br, bl, blendmode);
}

void AnimScript::set_parent_data(float x, float y, float z, float rx, float ry, float rz, float sx, float sy)
{
    px = x;
    py = y;
    pz = z;
    prx = rx;
    pry = ry;
    prz = rz;
    psx = sx;
    psy = sy;
}

void AnimScript::visible(bool visible)
{
    if (visible) flags |= ANM_FLAG_VISIBLE;
    else flags &= (~ANM_FLAG_VISIBLE);
}
void AnimScript::flip_x()
{
    flags ^= ANM_FLAG_FLIP_X;
}
void AnimScript::flip_y()
{
    flags ^= ANM_FLAG_FLIP_Y;
}
void AnimScript::disable3d(bool i)
{
    if (i) flags |= ANM_FLAG_DISABLE_3D;
    else flags &= (~ANM_FLAG_DISABLE_3D);
}
void AnimScript::colorizeChildren(bool i)
{
    if (i) flags |= ANM_FLAG_COLCHILD;
    else flags &= (~ANM_FLAG_COLCHILD);
}
void AnimScript::IsGui(bool gui)
{
    if (gui) flags |= ANM_FLAG_GUI;
    else flags &= (~ANM_FLAG_GUI);
}
void AnimScript::special_draw(uint8_t i)
{
    specialdraw = i;
}
void AnimScript::set_sprite(uint16_t s)
{
    sprit = s+1;
}
void AnimScript::offset_sprite(int16_t o)
{
    sprit+=o;
}
void AnimScript::set_layer(uint8_t l)
{
    lyr = l+1;
}
void AnimScript::set_blendmode(uint8_t i )
{
    blendmode = i;
}
void AnimScript::set_pos(float x, float y, float z)
{
    NSEngine::InterpolateManager::RemoveReference(&pos);
    pos = glm::vec3(x, y, z);
}
void AnimScript::set_rot(float x, float y, float z)
{
    NSEngine::InterpolateManager::RemoveReference(&rot);
    rot = glm::vec3(x, y, z);
}
void AnimScript::set_scale(float x, float y)
{
    NSEngine::InterpolateManager::RemoveReference(&scale);
    scale = glm::vec2(x, y);
}
void AnimScript::set_alpha(uint8_t a)
{
    NSEngine::InterpolateManager::RemoveReference(&this->a);
    this->a = a;
}
void AnimScript::set_color(uint8_t r, uint8_t g, uint8_t b)
{
    NSEngine::InterpolateManager::RemoveReference(&col1);
    col1 = NSEngine::ColorRGB(r, g, b);
}
void AnimScript::set_alpha2(uint8_t a)
{
    NSEngine::InterpolateManager::RemoveReference(&a2);
    a2 = a;
}
void AnimScript::set_color2(uint8_t r, uint8_t g, uint8_t b)
{
    NSEngine::InterpolateManager::RemoveReference(&col2);
    col2 = NSEngine::ColorRGB(r, g, b);
}
void AnimScript::pos_time(uint16_t t, uint8_t mode, float x, float y, float z)
{
    ITP_ (&pos, pos, glm::vec3(x, y, z), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::color_time(uint16_t t, uint8_t mode, uint8_t r, uint8_t g, uint8_t b)
{
    ITP_ (&col1, col1, NSEngine::ColorRGB(r, g, b), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::alpha_time(uint16_t t, uint8_t mode, uint8_t a)
{
    ITP_ (&(this->a), this->a, ituint8(a), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::rot_time(uint16_t t, uint8_t mode, float x, float y, float z)
{
    ITP_ (&rot, rot, glm::vec3(x, y, z), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::scale_time(uint16_t t, uint8_t mode, float x, float y)
{
    ITP_ (&scale, scale, glm::vec2(x, y), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::color2_time(uint16_t t, uint8_t mode, uint8_t r, uint8_t g, uint8_t b)
{
    ITP_ (&col2, col2, NSEngine::ColorRGB(r, g, b), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::alpha2_time(uint16_t t, uint8_t mode, uint8_t a)
{
    ITP_ (&a2, a2, ituint8(a), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::rot_vel(float x, float y, float z)
{
    rx_vel = x;
    ry_vel = y;
    rz_vel = z;
}
void AnimScript::scale_vel(float x, float y)
{
    sx_vel = x;
    sy_vel = y;
}
void AnimScript::move_bezier(uint16_t t, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    NSEngine::InterpolateManager::InterpolateBezier(&pos, pos, {x2, y2, z2}, {x1, y1, z1}, {x3, y3, z3}, t);
}
void AnimScript::scroll_x(float amt)
{
    u_vel = -amt;
}
void AnimScript::scroll_y(float amt)
{
    v_vel = -amt;
}
void AnimScript::scroll_x_time(uint16_t t, uint8_t mode, float amt)
{
    ITP_ (&u_vel, u_vel, amt, mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::scroll_y_time(uint16_t t, uint8_t mode, float amt)
{
    ITP_ (&v_vel, v_vel, amt, mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::zoom(float u, float v)
{
    uvz = glm::vec2(u, v);
}
void AnimScript::zoom_time(uint16_t t, uint8_t mode, float u, float v)
{
    ITP_ (&uvz, uvz, glm::vec2(u, v), mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::anchor(uint8_t h, uint8_t v)
{
    flag(ANM_FLAG_OFFL,h&1);
    flag(ANM_FLAG_OFFR,h&2);
    flag(ANM_FLAG_OFFT,v&1);
    flag(ANM_FLAG_OFFB,v&2);
}
void AnimScript::anchor_offset(float x, float y)
{
    ox = x;
    oy = y;
}
void AnimScript::raise_interrupt(uint8_t i)
{
    interrupt = i;
}
void AnimScript::x_time(uint16_t t, uint8_t mode, float x)
{
    ITP_ (&(pos.x), pos.x, x, mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::y_time(uint16_t t, uint8_t mode, float y)
{
    ITP_ (&(pos.y), pos.y, y, mode, t, flags & ANM_FLAG_GUI);
}
void AnimScript::z_time(uint16_t t, uint8_t mode, float z)
{
    ITP_ (&(pos.z), pos.z, z, mode, t, flags & ANM_FLAG_GUI);
}
float AnimScript::x_(bool p)
{
    return p?px:pos.x;
}
float AnimScript::y_(bool p)
{
    return p?py:pos.y;
}
float AnimScript::z_(bool p)
{
    return p?pz:pos.z;
}
void AnimScript::incx(float x)
{
    pos.x += x;
}
void AnimScript::incy(float y)
{
    pos.y += y;
}
void AnimScript::incz(float z)
{
    pos.z += z;
}
float AnimScript::xs_()
{
    return scale.x;
}
float AnimScript::ys_()
{
    return scale.y;
}
int AnimScript::spr_() 
{
    return sprit;
}

bool AnimScript::flag(uint16_t f)
{
    return flags&f;
}
void AnimScript::flag(uint16_t f, bool set)
{
    if (set) flags |= f;
    else flags &= ~f;
}
