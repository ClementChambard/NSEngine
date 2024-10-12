#include "DrawFuncs.h"
#include "math/math.hpp"

namespace ns {

static Color defaultDrawColor = c_white;

void draw_set_color(Color c) {
    defaultDrawColor = c;
}

void draw_set_color(u8 r, u8 g, u8 b, u8 a) {
    defaultDrawColor = Color(r, g, b, a);
}

void draw_set_color(u8 r, u8 g, u8 b) {
    defaultDrawColor.r = r; defaultDrawColor.g = g; defaultDrawColor.b = b;
}

void draw_set_alpha(u8 a) {
    defaultDrawColor.a = a;
}

void batch_draw_line(DrawBatch* batch, i32 x1, i32 y1, i32 x2, i32 y2, i32 width) {
    f32 angle = math::point_direction(x1,y1,x2,y2);
    f32 dirx = math::lengthdir_x((f32)width / 2.f, angle + ns::PI<f32> / 2.f);
    f32 diry = math::lengthdir_y((f32)width / 2.f, angle + ns::PI<f32> / 2.f);
    Vertex g1 = {{(f32)x1 - dirx,(f32)y1 - diry,0.f}, defaultDrawColor, {0,0}};
    Vertex d1 = {{(f32)x1 + dirx,(f32)y1 + diry,0.f}, defaultDrawColor, {1,0}};
    Vertex d2 = {{(f32)x2 + dirx,(f32)y2 + diry,0.f}, defaultDrawColor, {1,1}};
    Vertex g2 = {{(f32)x2 - dirx,(f32)y2 - diry,0.f}, defaultDrawColor, {0,1}};
    batch->draw_quad(g1,d1,d2,g2);
}

void batch_draw_line_color(DrawBatch* batch, i32 x1, i32 y1, i32 x2, i32 y2, i32 width, Color c1, Color c2) {
    f32 angle = math::point_direction(x1,y1,x2,y2);
    f32 dirx = math::lengthdir_x((f32)width / 2.f, angle + 90);
    f32 diry = math::lengthdir_y((f32)width / 2.f, angle + 90);
    Vertex g1 = {{(f32)x1 - dirx,(f32)y1 - diry,0.f}, c1, {0,0}};
    Vertex d1 = {{(f32)x1 + dirx,(f32)y1 + diry,0.f}, c1, {1,0}};
    Vertex d2 = {{(f32)x2 + dirx,(f32)y2 + diry,0.f}, c2, {1,1}};
    Vertex g2 = {{(f32)x2 - dirx,(f32)y2 - diry,0.f}, c2, {0,1}};
    batch->draw_quad(g1,d1,d2,g2);
}

void batch_draw_rectangle(DrawBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, bool outline) {
    batch_draw_rectangle_color(batch, x1, y1, x2, y2, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
}

void batch_draw_rectangle_color(DrawBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline) {
    if (outline) {
        batch_draw_line_color(batch, x1, y1, x1, y2, 1, ctl, cbl);
        batch_draw_line_color(batch, x1, y1, x2, y1, 1, ctl, ctr);
        batch_draw_line_color(batch, x1, y2, x2, y2, 1, cbl, cbr);
        batch_draw_line_color(batch, x2, y1, x2, y2, 1, ctr, cbr);
        return;
    }
    Vertex tl = {{(f32)x1,(f32)y1,0.f}, ctl, {0,0}};
    Vertex tr = {{(f32)x2,(f32)y1,0.f}, ctr, {1,0}};
    Vertex br = {{(f32)x2,(f32)y2,0.f}, cbr, {1,1}};
    Vertex bl = {{(f32)x1,(f32)y2,0.f}, cbl, {0,1}};
    batch->draw_quad(tl,tr,br,bl);
}

void batch_draw_rectangle_rotated(DrawBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, bool outline) {
    batch_draw_rectangle_rotated_color(batch, cx, cy, w, h, rotation, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
}

void batch_draw_rectangle_rotated_color(DrawBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, Color ctl, Color ctr, Color cbr, Color cbl, bool outline) {
    mat4 rot = mat4::mk_rotate_z(rotation);
    vec2 tl = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4(-w/2.f, h/2.f, 0, 0)));
    vec2 tr = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4( w/2.f, h/2.f, 0, 0)));
    vec2 br = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4( w/2.f,-h/2.f, 0, 0)));
    vec2 bl = (vec2)(vec4(cx, cy, 0, 0) + (rot * vec4(-w/2.f,-h/2.f, 0, 0)));
    batch_draw_quad_color_2d(batch, tl, tr, br, bl, ctl, ctr, cbr, cbl, outline);
}

void batch_draw_quad(DrawBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl) {
    batch_draw_quad_color(batch, tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor);
}

void batch_draw_quad_color(DrawBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl) {
    Vertex vtl = {tl, ctl, {0,0}};
    Vertex vtr = {tr, ctr, {1,0}};
    Vertex vbr = {br, cbr, {1,1}};
    Vertex vbl = {bl, cbl, {0,1}};
    batch->draw_quad(vtl,vtr,vbr,vbl);
}

void batch_draw_quad_2d(DrawBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, bool outline) {
    batch_draw_quad_color_2d(batch, tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
}

void batch_draw_quad_color_2d(DrawBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline) {
    if (outline) {
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
    batch->draw_quad(vtl,vtr,vbr,vbl);
}

void batch_draw_triangle(DrawBatch* batch, vec3 t1, vec3 t2, vec3 t3) {
    batch_draw_triangle_color(batch, t1, t2, t3, defaultDrawColor, defaultDrawColor, defaultDrawColor);
}

void batch_draw_triangle_color(DrawBatch* batch, vec3 t1, vec3 t2, vec3 t3, Color ct1, Color ct2, Color ct3) {
    batch->draw_tri(
        {t1, ct1, {}},
        {t2, ct2, {}},
        {t3, ct3, {}}
    );
}

void batch_draw_aa_box(DrawBatch* batch, vec3 p1, vec3 p2, Color c, bool shade)
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
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y2,z1},{x2,y2,z1},{x2,y1,z1},c1,c1,c1,c1);
    batch_draw_quad_color(batch, {x1,y1,z2},{x1,y2,z2},{x2,y2,z2},{x2,y1,z2},c1,c1,c1,c1);
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y2,z1},{x1,y2,z2},{x1,y1,z2},c2,c2,c2,c2);
    batch_draw_quad_color(batch, {x2,y1,z1},{x2,y2,z1},{x2,y2,z2},{x2,y1,z2},c2,c2,c2,c2);
    batch_draw_quad_color(batch, {x1,y1,z1},{x1,y1,z2},{x2,y1,z2},{x2,y1,z1},c,c,c,c);
    batch_draw_quad_color(batch, {x1,y2,z1},{x1,y2,z2},{x2,y2,z2},{x2,y2,z1},c,c,c,c);
}

static i32 circleverteces = 90;

void draw_set_circle_vertex_count(i32 verteces)
{
    circleverteces = verteces+verteces%2;
}

void batch_draw_circle(DrawBatch* batch, i32 x, i32 y, f32 r, bool outline) {
    batch_draw_circle_color(batch, x, y, r, defaultDrawColor, defaultDrawColor, outline);
}

void batch_draw_circle_color(DrawBatch* batch, i32 x, i32 y, f32 r, Color c_in, Color c_out, bool outline) {
    Vertex c = {{(f32)x, (f32)y, 0.f}, c_in, {0,0}};
    Vertex v1 = {{}, c_out, {1,0}}, v2 = {{}, c_out, {1,1}}, v3 = {{}, c_out, {0,1}};
    double circleStep = ns::PI_2<f32>/(f32)circleverteces;
    for (i32 i = 0; i < circleverteces/2; i++) {
        f32 a = 2*i*circleStep;
        v1.position = {x + math::lengthdir_x(r,a),y + math::lengthdir_y(r,a),0.f};
        v2.position = {x + math::lengthdir_x(r,a+circleStep),y + math::lengthdir_y(r,a+circleStep),0.f};
        v3.position = {x + math::lengthdir_x(r,a+2*circleStep),y + math::lengthdir_y(r,a+2*circleStep),0.f};
        if (outline)
        {
            batch_draw_line_color(batch, v1.position.x,v1.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
            batch_draw_line_color(batch, v3.position.x,v3.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
            continue;
        }
        batch->draw_quad(c,v1,v2,v3);
    }
}

void batch_draw_circle_arc(DrawBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2) {
    batch_draw_circle_arc_color(batch, x, y, r1, r2, a1, a2, defaultDrawColor, defaultDrawColor);
}

void batch_draw_circle_arc_color(DrawBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, Color c_in, Color c_out) {
    Vertex v0 = {{}, c_out, {0,0}}, v1 = {{}, c_in, {1,0}}, v2 = {{}, c_in, {1,1}}, v3 = {{}, c_out, {0,1}};
    double circleStep = ns::PI_2<f32>/(f32)circleverteces;
    for (double a = a1; a < a2; a += circleStep) {
        v0.position = {x + math::lengthdir_x(r2, a), y + math::lengthdir_y(r2, a), 0.f};
        v1.position = {x + math::lengthdir_x(r1, a), y + math::lengthdir_y(r1, a), 0.f};
        v2.position = {x + math::lengthdir_x(r1, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r1, math::min(a2,(f32)(a+circleStep))), 0.f};
        v3.position = {x + math::lengthdir_x(r2, math::min(a2,(f32)(a+circleStep))), y + math::lengthdir_y(r2, math::min(a2,(f32)(a+circleStep))), 0.f};
        batch->draw_quad(v0,v1,v2,v3);
    }
}

static vec3 getCylPos(f32 r, f32 h, f32 a) {
    vec3 pos;
    pos.x = r * cos(a);
    pos.y = h;
    pos.z = r * sin(a);
    return pos;
}

void batch_draw_cylinder(DrawBatch* batch, vec3 pos, vec3 rot, f32 r, f32 h, f32 a1, f32 a2, f32 u1, f32 u2, i32 repetitions) {
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
        batch->draw_quad(v0,v1,v2,v3);
        i++;
    }
}


}
