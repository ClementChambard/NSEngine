#include "DrawFuncs.h"

#include "math/math.hpp"
#include "TextureManager.h"
#include "logger.h"
#include "NSEngine.hpp"

#ifndef transfPos12
#define transfPos12(a) {a.x * xs, a.y * ys, a.z}
#endif
#ifndef transfPos2
#define transfPos2(a) {a.x + x, a.y + y, a.z}
#endif

namespace ns {

static Color defaultDrawColor = c_white;
static i32 defaultBlendMode = 0;
static i32 targetLayer = 0;

void draw_set_color(Color c)
{
    defaultDrawColor = c;
}

void draw_set_color(u8 r, u8 g, u8 b, u8 a)
{
    defaultDrawColor = Color(r, g, b, a);
}

void draw_set_color(u8 r, u8 g, u8 b)
{
    defaultDrawColor.r = r; defaultDrawColor.g = g; defaultDrawColor.b = b;
}

void draw_set_alpha(u8 a)
{
    defaultDrawColor.a = a;
}

void draw_set_blend(i32 bm)
{
    defaultBlendMode = bm; //Sprite::blendmode = bm;
}

void draw_set_layer(usize layerID) {
  if (layerID >= get_game_layers().size()) {
    NS_ERROR("no such graphics layer");
    return;
  }
  targetLayer = layerID;
}

void draw_line(i32 x1, i32 y1, i32 x2, i32 y2, i32 width)
{ batch_draw_line(&get_game_layers()[targetLayer], x1, y1, x2, y2, width); }

void batch_draw_line(SpriteBatch* batch, i32 x1, i32 y1, i32 x2, i32 y2, i32 width)
{
    f32 angle = math::point_direction(x1,y1,x2,y2);
    f32 dirx = math::lengthdir_x((f32)width / 2.f, angle + ns::PI<f32> / 2.f);
    f32 diry = math::lengthdir_y((f32)width / 2.f, angle + ns::PI<f32> / 2.f);
    Vertex g1 = {{(f32)x1 - dirx,(f32)y1 - diry,0.f}, defaultDrawColor, {0,0}};
    Vertex d1 = {{(f32)x1 + dirx,(f32)y1 + diry,0.f}, defaultDrawColor, {1,0}};
    Vertex d2 = {{(f32)x2 + dirx,(f32)y2 + diry,0.f}, defaultDrawColor, {1,1}};
    Vertex g2 = {{(f32)x2 - dirx,(f32)y2 - diry,0.f}, defaultDrawColor, {0,1}};
    batch->draw(1,g1,d1,d2,g2,defaultBlendMode);
}

void draw_line_color(i32 x1, i32 y1, i32 x2, i32 y2, i32 width, Color c1, Color c2)
{ batch_draw_line_color(&get_game_layers()[targetLayer], x1, y1, x2, y2, width, c1, c2); }

void batch_draw_line_color(SpriteBatch* batch, i32 x1, i32 y1, i32 x2, i32 y2, i32 width, Color c1, Color c2)
{
    f32 angle = math::point_direction(x1,y1,x2,y2);
    f32 dirx = math::lengthdir_x((f32)width / 2.f, angle + 90);
    f32 diry = math::lengthdir_y((f32)width / 2.f, angle + 90);
    Vertex g1 = {{(f32)x1 - dirx,(f32)y1 - diry,0.f}, c1, {0,0}};
    Vertex d1 = {{(f32)x1 + dirx,(f32)y1 + diry,0.f}, c1, {1,0}};
    Vertex d2 = {{(f32)x2 + dirx,(f32)y2 + diry,0.f}, c2, {1,1}};
    Vertex g2 = {{(f32)x2 - dirx,(f32)y2 - diry,0.f}, c2, {0,1}};
    batch->draw(1,g1,d1,d2,g2,defaultBlendMode);
}

void draw_rectangle(f32 x1, f32 y1, f32 x2, f32 y2, bool outline)
{ batch_draw_rectangle(&get_game_layers()[targetLayer], x1, y1, x2, y2, outline); }

void batch_draw_rectangle(SpriteBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, bool outline)
{ batch_draw_rectangle_color(batch, x1, y1, x2, y2, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline); }
void draw_rectangle_color(f32 x1, f32 y1, f32 x2, f32 y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{ batch_draw_rectangle_color(&get_game_layers()[targetLayer], x1, y1, x2, y2, ctl, ctr, cbr, cbl, outline); }

void batch_draw_rectangle_color(SpriteBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{
    if (outline)
    {
        draw_line_color(x1,y1,x1,y2,1,ctl,cbl);
        draw_line_color(x1,y1,x2,y1,1,ctl,ctr);
        draw_line_color(x1,y2,x2,y2,1,cbl,cbr);
        draw_line_color(x2,y1,x2,y2,1,ctr,cbr);
        return;
    }
    Vertex tl = {{(f32)x1,(f32)y1,0.f}, ctl, {0,0}};
    Vertex tr = {{(f32)x2,(f32)y1,0.f}, ctr, {1,0}};
    Vertex br = {{(f32)x2,(f32)y2,0.f}, cbr, {1,1}};
    Vertex bl = {{(f32)x1,(f32)y2,0.f}, cbl, {0,1}};
    batch->draw(1,tl,tr,br,bl,defaultBlendMode);
}


void draw_rectangle_rotated(f32 cx, f32 cy, f32 w, f32 h, f32 rotation, bool outline)
{ batch_draw_rectangle_rotated(&get_game_layers()[targetLayer], cx, cy, w,  h, rotation, outline); }

void batch_draw_rectangle_rotated(SpriteBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, bool outline)
{ batch_draw_rectangle_rotated_color(batch, cx, cy, w, h, rotation, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline); }

void draw_rectangle_rotated_color(f32 cx, f32 cy, f32 w, f32 h, f32 rotation, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{ batch_draw_rectangle_rotated_color(&get_game_layers()[targetLayer], cx, cy, w,  h, rotation, ctl, ctr, cbr, cbl, outline); }

void batch_draw_rectangle_rotated_color(SpriteBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{
    mat4 rot = mat4::mk_rotate_z(rotation);
    vec2 tl = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4(-w/2.f, h/2.f, 0, 0)));
    vec2 tr = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4( w/2.f, h/2.f, 0, 0)));
    vec2 br = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4( w/2.f,-h/2.f, 0, 0)));
    vec2 bl = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4(-w/2.f,-h/2.f, 0, 0)));
    batch_draw_quad_color_2d(batch, tl, tr, br, bl, ctl, ctr, cbr, cbl, outline);
}

void draw_quad(vec3 tl, vec3 tr, vec3 br, vec3 bl, bool)
{ batch_draw_quad(&get_game_layers()[targetLayer], tl, tr, br, bl); }

void batch_draw_quad(SpriteBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl, bool)
{ batch_draw_quad_color(batch, tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor); }

void draw_quad_color(vec3 tl, vec3 tr, vec3 br, vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool)
{ batch_draw_quad_color(&get_game_layers()[targetLayer], tl, tr, br, bl, ctl, ctr, cbr, cbl); }

void batch_draw_quad_color(SpriteBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool)
{
    Vertex vtl = {tl, ctl, {0,0}};
    Vertex vtr = {tr, ctr, {1,0}};
    Vertex vbr = {br, cbr, {1,1}};
    Vertex vbl = {bl, cbl, {0,1}};
    batch->draw(1,vtl,vtr,vbr,vbl,defaultBlendMode);
}

void draw_quad_2d(vec2 tl, vec2 tr, vec2 br, vec2 bl, bool outline)
{ batch_draw_quad_2d(&get_game_layers()[targetLayer], tl, tr, br, bl, outline); }

void batch_draw_quad_2d(SpriteBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, bool outline)
{ batch_draw_quad_color_2d(batch, tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline); }

void draw_quad_color_2d(vec2 tl, vec2 tr, vec2 br, vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{ batch_draw_quad_color_2d(&get_game_layers()[targetLayer], tl, tr, br, bl, ctl, ctr, cbr, cbl, outline); }

void batch_draw_quad_color_2d(SpriteBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
{
    if (outline)
    {
        batch_draw_line_color(batch,tl.x,tl.y,tr.x,tr.y,1,ctl,ctr);
        batch_draw_line_color(batch,tl.x,tl.y,bl.x,bl.y,1,ctl,cbl);
        batch_draw_line_color(batch,tr.x,tr.y,br.x,br.y,1,ctr,cbr);
        batch_draw_line_color(batch,bl.x,bl.y,br.x,br.y,1,cbl,cbr);
        return;
    }
    Vertex vtl = {{tl, 0}, ctl, {0,0}};
    Vertex vtr = {{tr, 0}, ctr, {1,0}};
    Vertex vbr = {{br, 0}, cbr, {1,1}};
    Vertex vbl = {{bl, 0}, cbl, {0,1}};
    batch->draw(1,vtl,vtr,vbr,vbl,defaultBlendMode);
}

void draw_quad_tex_2d(u32 texID, vec4 tl, vec4 tr, vec4 br, vec4 bl, bool outline)
{ batch_draw_quad_tex_2d(&get_game_layers()[targetLayer], texID, tl, tr, br, bl, outline); }

void batch_draw_quad_tex_2d(SpriteBatch* batch, u32 texID, vec4 tl, vec4 tr, vec4 br, vec4 bl, bool)
{
    Vertex vtl = {{tl.x,tl.y,0}, defaultDrawColor, {tl.z,tl.w}};
    Vertex vtr = {{tr.x,tr.y,0}, defaultDrawColor, {tr.z,tr.w}};
    Vertex vbr = {{br.x,br.y,0}, defaultDrawColor, {br.z,br.w}};
    Vertex vbl = {{bl.x,bl.y,0}, defaultDrawColor, {bl.z,bl.w}};
    batch->draw(texID,vtl,vtr,vbr,vbl,defaultBlendMode);
}

void draw_triangle(vec3 t1, vec3 t2, vec3 t3, bool outline)
{ batch_draw_triangle(&get_game_layers()[targetLayer], t1, t2, t3, outline); }

void batch_draw_triangle(SpriteBatch* batch, vec3 t1, vec3 t2, vec3 t3, bool outline)
{ batch_draw_triangle_color(batch, t1, t2, t3, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline); }

void draw_triangle_color(vec3 t1, vec3 t2, vec3 t3, Color ct1, Color ct2, Color ct3, bool outline)
{ batch_draw_triangle_color(&get_game_layers()[targetLayer], t1, t2, t3, ct1, ct2, ct3, outline); }

void batch_draw_triangle_color(SpriteBatch* batch, vec3 t1, vec3 t2, vec3 t3, Color ct1, Color ct2, Color ct3, bool outline)
{ batch_draw_quad_color(batch, t1,t1,t2,t3,ct1,ct1,ct2,ct3,outline); }


void draw_aa_box(vec3 p1, vec3 p2, Color c, bool, bool shade)
{ batch_draw_aa_box(&get_game_layers()[targetLayer], p1, p2, c, false, shade); }

void batch_draw_aa_box(SpriteBatch* batch, vec3 p1, vec3 p2, Color c, bool, bool shade)
{
    f32 x1 = p1.x;
    f32 y1 = p1.y;
    f32 z1 = p1.z;
    f32 x2 = p2.x;
    f32 y2 = p2.y;
    f32 z2 = p2.z;
    Color c1 = c, c2 = c;
    if (shade)
    {
        c1 /= 1.5;
        c1.a = c.a;
        c2 /= 2;
        c2.a = c.a;
    }
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y2,z1},{x2,y2,z1},{x2,y1,z1},c1,c1,c1,c1,false);
    batch_draw_quad_color(batch, {x1,y1,z2},{x1,y2,z2},{x2,y2,z2},{x2,y1,z2},c1,c1,c1,c1,false);
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y2,z1},{x1,y2,z2},{x1,y1,z2},c2,c2,c2,c2,false);
    batch_draw_quad_color(batch, {x2,y1,z1},{x2,y2,z1},{x2,y2,z2},{x2,y1,z2},c2,c2,c2,c2,false);
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y1,z2},{x2,y1,z2},{x2,y1,z1},c,c,c,c,false);
    batch_draw_quad_color(batch, {x1,y2,z1},{x1,y2,z2},{x2,y2,z2},{x2,y2,z1},c,c,c,c,false);
}

static i32 circleverteces = 90;

void draw_set_circle_vertex_count(i32 verteces)
{
    circleverteces = verteces+verteces%2;
}

void draw_circle(i32 x, i32 y, f32 r, bool outline)
{ batch_draw_circle(&get_game_layers()[targetLayer], x, y, r, outline); }

void batch_draw_circle(SpriteBatch* batch, i32 x, i32 y, f32 r, bool outline)
{ batch_draw_circle_color(batch, x, y, r, defaultDrawColor, defaultDrawColor, outline); }

void draw_circle_color(i32 x, i32 y, f32 r, Color c_in, Color c_out, bool outline)
{ batch_draw_circle_color(&get_game_layers()[targetLayer], x, y, r, c_in, c_out, outline); }

void batch_draw_circle_color(SpriteBatch* batch, i32 x, i32 y, f32 r, Color c_in, Color c_out, bool outline)
{
    Vertex c = {{(f32)x, (f32)y, 0.f}, c_in, {0,0}};
    Vertex v1 = {{}, c_out, {1,0}}, v2 = {{}, c_out, {1,1}}, v3 = {{}, c_out, {0,1}};
    double circleStep = ns::PI_2<f32>/(f32)circleverteces;
    for (i32 i = 0; i < circleverteces/2; i++)
    {
        f32 a = 2*i*circleStep;
        v1.position = {x + math::lengthdir_x(r,a),y + math::lengthdir_y(r,a),0.f};
        v2.position = {x + math::lengthdir_x(r,a+circleStep),y + math::lengthdir_y(r,a+circleStep),0.f};
        v3.position = {x + math::lengthdir_x(r,a+2*circleStep),y + math::lengthdir_y(r,a+2*circleStep),0.f};
        if (outline)
        {
            draw_line_color(v1.position.x,v1.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
            draw_line_color(v3.position.x,v3.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
            continue;
        }
        batch->draw(1,c,v1,v2,v3,defaultBlendMode);
    }
}

void draw_circle_arc(i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2)
{ batch_draw_circle_arc(&get_game_layers()[targetLayer], x, y, r1, r2, a1, a2); }

void batch_draw_circle_arc(SpriteBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2)
{ batch_draw_circle_arc_color(batch, x, y, r1, r2, a1, a2, defaultDrawColor, defaultDrawColor); }

void draw_circle_arc_color(i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, Color c_in, Color c_out)
{ batch_draw_circle_arc_color(&get_game_layers()[targetLayer], x, y, r1, r2, a1, a2, c_in, c_out); }

void batch_draw_circle_arc_color(SpriteBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, Color c_in, Color c_out)
{
    Vertex v0 = {{}, c_out, {0,0}}, v1 = {{}, c_in, {1,0}}, v2 = {{}, c_in, {1,1}}, v3 = {{}, c_out, {0,1}};
    double circleStep = ns::PI_2<f32>/(f32)circleverteces;
    for (double a = a1; a < a2; a += circleStep)
    {
        v0.position = {x + math::lengthdir_x(r2, a), y + math::lengthdir_y(r2, a), 0.f};
        v1.position = {x + math::lengthdir_x(r1, a), y + math::lengthdir_y(r1, a), 0.f};
        v2.position = {x + math::lengthdir_x(r1, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r1, math::min(a2,(f32)(a+circleStep))), 0.f};
        v3.position = {x + math::lengthdir_x(r2, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r2, math::min(a2,(f32)(a+circleStep))), 0.f};
        batch->draw(1,v0,v1,v2,v3,defaultBlendMode);
    }
}

//assumes the texture is vertically repeatable
void draw_circle_arc_textured(i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, i32 texID, f32 u1, f32 u2, i32 repetitions)
{ batch_draw_circle_arc_textured(&get_game_layers()[targetLayer], x, y, r1, r2, a1, a2, texID, u1, u2, repetitions); }

void batch_draw_circle_arc_textured(SpriteBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, i32 texID, f32 u1, f32 u2, i32 repetitions)
{
    double circleStep = ns::PI_2<f32>/(f32)circleverteces;
    f32 vStep = repetitions / (2 * ns::PI<f32> / circleStep);
    Vertex v0 = {{}, defaultDrawColor, {u1,0}}, v1 = {{}, defaultDrawColor, {u2,0}},
           v2 = {{}, defaultDrawColor, {u2,vStep}}, v3 = {{}, defaultDrawColor, {u1,vStep}};
    i32 i = 0;
    for (double a = a1; a < a2; a += circleStep)
    {
        v0.position = {x + math::lengthdir_x(r2, a), y + math::lengthdir_y(r2, a), 0.f};
        v0.uv.y = i * vStep;
        v1.position = {x + math::lengthdir_x(r1, a), y + math::lengthdir_y(r1, a), 0.f};
        v1.uv.y = i * vStep;
        v2.position = {x + math::lengthdir_x(r1, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r1, math::min(a2,(f32)(a+circleStep))), 0.f};
        v2.uv.y = (i+1) * vStep;
        v3.position = {x + math::lengthdir_x(r2, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r2, math::min(a2,(f32)(a+circleStep))), 0.f};
        v3.uv.y = (i+1) * vStep;
        batch->draw(texID,v0,v1,v2,v3,defaultBlendMode);
        i++;
    }
}

vec3 getCylPos(f32 r, f32 h, f32 a)
{
    vec3 pos;
    pos.x = r * cos(a);
    pos.y = h;
    pos.z = r * sin(a);
    return pos;
}

void draw_cylinder(vec3 pos, vec3 rot, f32 r, f32 h, f32 a1, f32 a2, i32 texID, f32 u1, f32 u2, i32 repetitions)
{ batch_draw_cylinder(&get_game_layers()[targetLayer], pos, rot, r, h, a1, a2, texID, u1, u2, repetitions); }

void batch_draw_cylinder(SpriteBatch* batch, vec3 pos, vec3 rot, f32 r, f32 h, f32 a1, f32 a2, i32 texID, f32 u1, f32 u2, i32 repetitions)
{
    double circleStep = abs(a2-a1)/(f32)circleverteces;
    f32 vStep = repetitions / (f32)circleverteces;
    Vertex v0 = {{}, defaultDrawColor, {u1,0}}, v1 = {{}, defaultDrawColor, {u2,0}},
           v2 = {{}, defaultDrawColor, {u2,vStep}}, v3 = {{}, defaultDrawColor, {u1,vStep}};
    i32 i = 0;
    vec4 pos4 = vec4(pos, 0);

    mat4 rotation(1.f);
    if (rot.z != 0.0) rotation = rotation * mat4::mk_rotate_z(rot.z);
    if (rot.y != 0.0) rotation = rotation * mat4::mk_rotate_y(rot.y);
    if (rot.x != 0.0) rotation = rotation * mat4::mk_rotate_z(rot.x);
    for (double a = a1; a < a2; a += circleStep)
    {
        f32 aa2 = math::min(a2,f32(a+circleStep));
        v0.position = pos4 + rotation * vec4(getCylPos(r, -h/2.f, a),1);
        v0.uv.y = i * vStep;
        v1.position = pos4 + rotation * vec4(getCylPos(r, h/2.f, a),1);
        v1.uv.y = i * vStep;
        v2.position = pos4 + rotation * vec4(getCylPos(r, h/2.f, aa2),1);
        v2.uv.y = (i+1) * vStep;
        v3.position = pos4 + rotation * vec4(getCylPos(r, -h/2.f, aa2),1);
        v3.uv.y = (i+1) * vStep;
        batch->draw(texID,v0,v1,v2,v3,defaultBlendMode);
        i++;
    }
}

void draw_line_3d(f32 x1, f32 y1, f32, f32 x2, f32 y2, f32, f32 width)
{ batch_draw_line_color(&get_game_layers()[targetLayer], x1, y1, x2, y2, width, defaultDrawColor, defaultDrawColor); }

void batch_draw_line_3d(SpriteBatch* batch, f32 x1, f32 y1, f32, f32 x2, f32 y2, f32, f32 width)
{ batch_draw_line_color(batch, x1, y1, x2, y2, width, defaultDrawColor, defaultDrawColor); }

void draw_line_color_3d(f32 x1, f32 y1, f32, f32 x2, f32 y2, f32, f32 width, Color c1, Color c2)
{ batch_draw_line_color(&get_game_layers()[targetLayer], x1, y1, x2, y2, width, c1, c2); }

void batch_draw_line_color_3d(SpriteBatch* batch, f32 x1, f32 y1, f32, f32 x2, f32 y2, f32, f32 width, Color c1, Color c2)
{
    // vec3 pos = vec3((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
    // vec3 axis = vec3(x1-x2, y1-y2, z1-z2);
    // vec2 size = vec2(width, sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2)));
    batch_draw_line_color(batch, x1, y1, x2, y2, width, c1, c2);
    // batch_draw_billboard_axis(batch, pos, axis, size, 1, vec4(0,0,1,1), c1, c1, c2, c2);
}

void draw_billboard(vec3 pos, vec2 size, i32 texID, vec4 uvs, Color tl, Color tr, Color br, Color bl)
{
    vec3 ViewX = vec3(1.0, 0.0, 0.0);
    vec3 ViewY = vec3(0.0, 1.0, 0.0);
    vec3 postl = pos - size.x/2 * ViewX + size.y/2 * ViewY;
    vec3 postr = pos + size.x/2 * ViewX + size.y/2 * ViewY;
    vec3 posbr = pos + size.x/2 * ViewX - size.y/2 * ViewY;
    vec3 posbl = pos - size.x/2 * ViewX - size.y/2 * ViewY;
    get_game_layers()[targetLayer].draw(texID,
            {postl, tl, {uvs.x,uvs.y}},
            {postr, tr, {uvs.z,uvs.y}},
            {posbr, br, {uvs.z,uvs.w}},
            {posbl, bl, {uvs.x,uvs.w}},
        defaultBlendMode);
}

void batch_draw_billboard(SpriteBatch* batch, vec3 pos, vec2 size, i32 texID, vec4 uvs, Color tl, Color tr, Color br, Color bl)
{
    vec3 ViewX = vec3(1.0, 0.0, 0.0);
    vec3 ViewY = vec3(0.0, 1.0, 0.0);
    vec3 postl = pos - size.x/2 * ViewX + size.y/2 * ViewY;
    vec3 postr = pos + size.x/2 * ViewX + size.y/2 * ViewY;
    vec3 posbr = pos + size.x/2 * ViewX - size.y/2 * ViewY;
    vec3 posbl = pos - size.x/2 * ViewX - size.y/2 * ViewY;
    batch->draw(texID,
            {postl, tl, {uvs.x,uvs.y}},
            {postr, tr, {uvs.z,uvs.y}},
            {posbr, br, {uvs.z,uvs.w}},
            {posbl, bl, {uvs.x,uvs.w}},
        defaultBlendMode);
}

void draw_billboard_axis(vec3 pos, vec3 axis, vec2 size, i32 texID, vec4 uvs, Color tl, Color tr, Color br, Color bl)
{ 
    axis.normalize();
    vec3 frontvec;
    if (axis.x != 0 && axis.y != 0) frontvec = vec3(axis.y,-axis.x,0).normalized();
    else if (axis.x != 0 && axis.z != 0) frontvec = vec3(axis.z,0,-axis.x).normalized();
    else if (axis.z != 0 && axis.y != 0) frontvec = vec3(0,-axis.z,axis.y).normalized();
    else if (axis.x != 0) frontvec = vec3(0,axis.x,0);
    else if (axis.y != 0) frontvec = vec3(axis.y,0,0);
    else if (axis.z != 0) frontvec = vec3(0,axis.z,0);
    vec3 rightvec = axis.cross(frontvec);
    vec3 camPos = vec3(0,0,0);
    vec3 objtocam = (camPos - pos).normalized();
    objtocam = (objtocam - (axis * objtocam.dot(axis))).normalized();
    // vec3 axisRot = frontvec.cross(objtocam);
    vec4 p = vec4(pos,0);
    // f32 ang = acos(frontvec.dot(objtocam));
    mat4 rot = mat4(1.0f); // rotate(mat4(1), ang, axisRot);
    vec4 postl = p + rot * vec4(-size.x/2 * rightvec + size.y/2 * axis,0);
    vec4 postr = p + rot * vec4(size.x/2 * rightvec + size.y/2 * axis,0);
    vec4 posbr = p + rot * vec4(size.x/2 * rightvec - size.y/2 * axis,0);
    vec4 posbl = p + rot * vec4(-size.x/2 * rightvec - size.y/2 * axis,0);
    get_game_layers()[targetLayer].draw(texID,
            {vec3(postl), tl, {uvs.x,uvs.y}},
            {vec3(postr), tr, {uvs.z,uvs.y}},
            {vec3(posbr), br, {uvs.z,uvs.w}},
            {vec3(posbl), bl, {uvs.x,uvs.w}},
        defaultBlendMode);
    }

void batch_draw_billboard_axis(SpriteBatch* batch, vec3 pos, vec3 axis, vec2 size, i32 texID, vec4 uvs, Color tl, Color tr, Color br, Color bl)
{
    axis.normalize();
    vec3 frontvec;
    if (axis.x != 0 && axis.y != 0) frontvec = vec3(axis.y,-axis.x,0).normalized();
    else if (axis.x != 0 && axis.z != 0) frontvec = vec3(axis.z,0,-axis.x).normalized();
    else if (axis.z != 0 && axis.y != 0) frontvec = vec3(0,-axis.z,axis.y).normalized();
    else if (axis.x != 0) frontvec = vec3(0,axis.x,0);
    else if (axis.y != 0) frontvec = vec3(axis.y,0,0);
    else if (axis.z != 0) frontvec = vec3(0,axis.z,0);
    vec3 rightvec = axis.cross(frontvec);
    vec3 camPos = vec3(0,0,0);
    vec3 objtocam = (camPos - pos).normalized();
    objtocam = (objtocam - (axis * objtocam.dot(axis))).normalized();
    // vec3 axisRot = frontvec.cross(objtocam);
    vec4 p = vec4(pos,0);
    // f32 ang = acos(frontvec.dot(objtocam));
    mat4 rot = mat4(1.f); // rotate(mat4(1), ang, axisRot);
    vec4 postl = p + rot * vec4(-size.x/2 * rightvec + size.y/2 * axis,0);
    vec4 postr = p + rot * vec4(size.x/2 * rightvec + size.y/2 * axis,0);
    vec4 posbr = p + rot * vec4(size.x/2 * rightvec - size.y/2 * axis,0);
    vec4 posbl = p + rot * vec4(-size.x/2 * rightvec - size.y/2 * axis,0);
    batch->draw(texID,
            {vec3(postl), tl, {uvs.x,uvs.y}},
            {vec3(postr), tr, {uvs.z,uvs.y}},
            {vec3(posbr), br, {uvs.z,uvs.w}},
            {vec3(posbl), bl, {uvs.x,uvs.w}},
        defaultBlendMode);
}

}
