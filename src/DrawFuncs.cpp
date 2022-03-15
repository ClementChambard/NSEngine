#include "DrawFuncs.h"
#include <glm/gtx/euler_angles.hpp>
#include "NSEngine.h"
#include "math/math.h"

#ifndef transfPos12
#define transfPos12(a) {a.x * xs, a.y * ys, a.z}
#endif
#ifndef transfPos2
#define transfPos2(a) {a.x + x, a.y + y, a.z}
#endif
namespace NSEngine {

    Color defaultDrawColor = c_white;
    int defaultBlendMode = 0;

    void draw_game_sprite(int sprite_index, int x, int y, float xs, float ys, float rot, Color ctl, Color ctr, Color cbr, Color cbl)
    {

        sprite_data* sd = &(GS_ sprs[sprite_index]);

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.f), rot, {0.f, 0.f, 1.f});

        glm::vec3 tl1 = glm::vec3(-xs*sd->w/2, -ys*sd->h/2, 0.f);
        glm::vec4 rotatedTL = glm::vec4(tl1,1.f) * rotationMatrix;
        glm::vec3 tr1 = glm::vec3(xs*sd->w/2, -ys*sd->h/2, 0.f);
        glm::vec4 rotatedTR = glm::vec4(tr1,1.f) * rotationMatrix;
        glm::vec3 br1 = glm::vec3(xs*sd->w/2, ys*sd->h/2, 0.f);
        glm::vec4 rotatedBR = glm::vec4(br1,1.f) * rotationMatrix;
        glm::vec3 bl1 = glm::vec3(-xs*sd->w/2, ys*sd->h/2, 0.f);
        glm::vec4 rotatedBL = glm::vec4(bl1,1.f) * rotationMatrix;

        Vertex TL = {transfPos2(rotatedTL), ctl, {sd->u1, sd->v2}};
        Vertex TR = {transfPos2(rotatedTR), ctr, {sd->u2, sd->v2}};
        Vertex BR = {transfPos2(rotatedBR), cbr, {sd->u2, sd->v1}};
        Vertex BL = {transfPos2(rotatedBL), cbl, {sd->u1, sd->v1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(sd->tex_id, TL, TR, BR, BL, defaultBlendMode);
    }

    void draw_sprite(int sprite_index, int frame, int x, int y)
    {
        SpriteManager::draw_sprite(sprite_index, frame, x, y, 1.f, 1.f, 0.f, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor);
    }

    void draw_sprite_scaled(int sprite_index, int frame, int x, int y, float xscale, float yscale)
    {
        SpriteManager::draw_sprite(sprite_index,frame,x,y,xscale,yscale, 0.f, defaultDrawColor,defaultDrawColor,defaultDrawColor,defaultDrawColor);
    }

    void draw_sprite_rotated(int sprite_index, int frame, int x, int y, float rot)
    {
        SpriteManager::draw_sprite(sprite_index,frame,x,y,1.f,1.f,rot,defaultDrawColor,defaultDrawColor,defaultDrawColor,defaultDrawColor);
    }

    void draw_sprite_scrotatled(int sprite_index, int frame, int x, int y, float xs, float ys, float rot)
    {
        SpriteManager::draw_sprite(sprite_index,frame,x,y,xs,ys,rot,defaultDrawColor,defaultDrawColor,defaultDrawColor,defaultDrawColor);
    }

    void draw_sprite_color(int sprite_index, int frame, int x, int y, Color c)
    {
        SpriteManager::draw_sprite(sprite_index, frame, x, y, 1.f, 1.f, 0.f, c, c, c, c);
    }

    void draw_sprite_scalor(int sprite_index, int frame, int x, int y, float xs, float ys, Color c)
    {
        SpriteManager::draw_sprite(sprite_index, frame, x, y, xs, ys, 0.f, c, c, c, c);
    }

    void draw_sprite_rotalor(int sprite_index, int frame, int x, int y, float rot, Color c)
    {
        SpriteManager::draw_sprite(sprite_index, frame, x, y, 1.f, 1.f, rot, c, c, c, c);
    }

    void draw_sprite_scrotalor(int sprite_index, int frame, int x, int y, float xs, float ys, float rot, Color c)
    {
        SpriteManager::draw_sprite(sprite_index, frame, x, y, xs, ys, rot, c, c, c, c);
    }

    void draw_set_color(Color c)
    {
        defaultDrawColor = c;
    }

    void draw_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    {
        defaultDrawColor = Color(r, g, b, a);
    }

    void draw_set_color(uint8_t r, uint8_t g, uint8_t b)
    {
        defaultDrawColor.r = r; defaultDrawColor.g = g; defaultDrawColor.b = b;
    }

    void draw_set_alpha(uint8_t a)
    {
        defaultDrawColor.a = a;
    }

    void draw_set_blend(int bm)
    {
        defaultBlendMode = bm; Sprite::blendmode = bm;
    }

    void draw_line(int x1, int y1, int x2, int y2, int width)
    {
        float angle = math::point_direction(x1,y1,x2,y2);
        float dirx = math::lengthdir_x((float)width / 2.f, angle + PI / 2.f);
        float diry = math::lengthdir_y((float)width / 2.f, angle + PI / 2.f);
        Vertex g1 = {{(float)x1 - dirx,(float)y1 - diry,0.f}, defaultDrawColor, {0,0}};
        Vertex d1 = {{(float)x1 + dirx,(float)y1 + diry,0.f}, defaultDrawColor, {1,0}};
        Vertex d2 = {{(float)x2 + dirx,(float)y2 + diry,0.f}, defaultDrawColor, {1,1}};
        Vertex g2 = {{(float)x2 - dirx,(float)y2 - diry,0.f}, defaultDrawColor, {0,1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),g1,d1,d2,g2,defaultBlendMode);
    }

    void draw_line_color(int x1, int y1, int x2, int y2, int width, Color c1, Color c2)
    {
        float angle = math::point_direction(x1,y1,x2,y2);
        float dirx = math::lengthdir_x((float)width / 2.f, angle + 90);
        float diry = math::lengthdir_y((float)width / 2.f, angle + 90);
        Vertex g1 = {{(float)x1 - dirx,(float)y1 - diry,0.f}, c1, {0,0}};
        Vertex d1 = {{(float)x1 + dirx,(float)y1 + diry,0.f}, c1, {1,0}};
        Vertex d2 = {{(float)x2 + dirx,(float)y2 + diry,0.f}, c2, {1,1}};
        Vertex g2 = {{(float)x2 - dirx,(float)y2 - diry,0.f}, c2, {0,1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),g1,d1,d2,g2,defaultBlendMode);
    }

    void draw_rectangle(float x1, float y1, float x2, float y2, bool outline)
    {
        draw_rectangle_color(x1, y1, x2, y2, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
    }

    void draw_rectangle_color(float x1, float y1, float x2, float y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
    {
        if (outline)
        {
            draw_line_color(x1,y1,x1,y2,1,ctl,cbl);
            draw_line_color(x1,y1,x2,y1,1,ctl,ctr);
            draw_line_color(x1,y2,x2,y2,1,cbl,cbr);
            draw_line_color(x2,y1,x2,y2,1,ctr,cbr);
            return;
        }
        Vertex tl = {{(float)x1,(float)y1,0.f}, ctl, {0,0}};
        Vertex tr = {{(float)x2,(float)y1,0.f}, ctr, {1,0}};
        Vertex br = {{(float)x2,(float)y2,0.f}, cbr, {1,1}};
        Vertex bl = {{(float)x1,(float)y2,0.f}, cbl, {0,1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),tl,tr,br,bl,defaultBlendMode);
    }

    void draw_quad(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, bool outline)
    {
        draw_quad_color(tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
    }

    void draw_quad_color(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
    {
        if (outline)
        {
            draw_line_color_3d(tl.x,tl.y,tl.z,tr.x,tr.y,tr.z,1,ctl,ctr);
            draw_line_color_3d(tl.x,tl.y,tl.z,bl.x,bl.y,bl.z,1,ctl,cbl);
            draw_line_color_3d(tr.x,tr.y,tr.z,br.x,br.y,br.z,1,ctr,cbr);
            draw_line_color_3d(bl.x,bl.y,bl.z,br.x,br.y,br.z,1,cbl,cbr);
            return;
        }
        Vertex vtl = {posfromvec(tl), ctl, {0,0}};
        Vertex vtr = {posfromvec(tr), ctr, {1,0}};
        Vertex vbr = {posfromvec(br), cbr, {1,1}};
        Vertex vbl = {posfromvec(bl), cbl, {0,1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),vtl,vtr,vbr,vbl,defaultBlendMode);
    }

    void draw_quad_2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, bool outline)
    {
        draw_quad_color_2d(tl, tr, br, bl, defaultDrawColor, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
    }

    void draw_quad_color_2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline)
    {
        if (outline)
        {
            draw_line_color(tl.x,tl.y,tr.x,tr.y,1,ctl,ctr);
            draw_line_color(tl.x,tl.y,bl.x,bl.y,1,ctl,cbl);
            draw_line_color(tr.x,tr.y,br.x,br.y,1,ctr,cbr);
            draw_line_color(bl.x,bl.y,br.x,br.y,1,cbl,cbr);
            return;
        }
        Vertex vtl = {posfromvec(tl), ctl, {0,0}};
        Vertex vtr = {posfromvec(tr), ctr, {1,0}};
        Vertex vbr = {posfromvec(br), cbr, {1,1}};
        Vertex vbl = {posfromvec(bl), cbl, {0,1}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),vtl,vtr,vbr,vbl,defaultBlendMode);
    }

    void draw_quad_tex_2d(GLuint texID, glm::vec4 tl, glm::vec4 tr, glm::vec4 br, glm::vec4 bl, bool outline)
    {
        Vertex vtl = {{tl.x,tl.y,0}, defaultDrawColor, {tl.z,tl.w}};
        Vertex vtr = {{tr.x,tr.y,0}, defaultDrawColor, {tr.z,tr.w}};
        Vertex vbr = {{br.x,br.y,0}, defaultDrawColor, {br.z,br.w}};
        Vertex vbl = {{bl.x,bl.y,0}, defaultDrawColor, {bl.z,bl.w}};
        engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,vtl,vtr,vbr,vbl,defaultBlendMode);
    }

    void draw_triangle(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, bool outline)
    {
        draw_triangle_color(t1, t2, t3, defaultDrawColor, defaultDrawColor, defaultDrawColor, outline);
    }

    void draw_triangle_color(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, Color ct1, Color ct2, Color ct3, bool outline)
    {
        draw_quad_color(t1,t1,t2,t3,ct1,ct1,ct2,ct3,outline);
    }


    void draw_AA_box(glm::vec3 p1, glm::vec3 p2, Color c, bool outline, bool shade)
    {
        float x1 = p1.x;
        float y1 = p1.y;
        float z1 = p1.z;
        float x2 = p2.x;
        float y2 = p2.y;
        float z2 = p2.z;
        Color c1 = c, c2 = c;
        if (shade)
        {
            c1.Darken(1.5);
            c2.Darken(2);
        }
        if (outline)
        {

        }
        draw_quad_color({x1,y1,z1},{x1,y2,z1},{x2,y2,z1},{x2,y1,z1},c1,c1,c1,c1,false);
        draw_quad_color({x1,y1,z2},{x1,y2,z2},{x2,y2,z2},{x2,y1,z2},c1,c1,c1,c1,false);
        draw_quad_color({x1,y1,z1},{x1,y2,z1},{x1,y2,z2},{x1,y1,z2},c2,c2,c2,c2,false);
        draw_quad_color({x2,y1,z1},{x2,y2,z1},{x2,y2,z2},{x2,y1,z2},c2,c2,c2,c2,false);
        draw_quad_color({x1,y1,z1},{x1,y1,z2},{x2,y1,z2},{x2,y1,z1},c,c,c,c,false);
        draw_quad_color({x1,y2,z1},{x1,y2,z2},{x2,y2,z2},{x2,y2,z1},c,c,c,c,false);
    }

    void draw_text(int font, const char* text, int x, int y)
    {
        SpriteManager::draw_text(font,text, glm::vec2(x,y), glm::vec2(1,1), defaultDrawColor);
    }

    int circleverteces = 90;

    void draw_circle_set_vertex_count(int verteces)
    {
        circleverteces = verteces+verteces%2;
    }

    void draw_circle(int x, int y, float r, bool outline)
    {
        draw_circle_color(x, y, r, defaultDrawColor, defaultDrawColor, outline);
    }

    void draw_circle_color(int x, int y, float r, Color c_in, Color c_out, bool outline)
    {
        Vertex c = {{(float)x, (float)y, 0.f}, c_in, {0,0}};
        Vertex v1 = {{}, c_out, {1,0}}, v2 = {{}, c_out, {1,1}}, v3 = {{}, c_out, {0,1}};
        double circleStep = PI2/(float)circleverteces;
        for (int i = 0; i < circleverteces/2; i++)
        {
            float a = 2*i*circleStep;
            v1.position = {x + math::lengthdir_x(r,a),y + math::lengthdir_y(r,a),0.f};
            v2.position = {x + math::lengthdir_x(r,a+circleStep),y + math::lengthdir_y(r,a+circleStep),0.f};
            v3.position = {x + math::lengthdir_x(r,a+2*circleStep),y + math::lengthdir_y(r,a+2*circleStep),0.f};
            if (outline)
            {
                draw_line_color(v1.position.x,v1.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
                draw_line_color(v3.position.x,v3.position.y,v2.position.x,v2.position.y,1,c_out,c_out);
                continue;
            }
            engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),c,v1,v2,v3,defaultBlendMode);
        }
    }

    void draw_circle_arc(int x, int y, float r1, float r2, float a1, float a2)
    {
        draw_circle_arc_color(x, y, r1, r2, a1, a2, defaultDrawColor, defaultDrawColor);
    }

    void draw_circle_arc_color(int x, int y, float r1, float r2, float a1, float a2, Color c_in, Color c_out)
    {
        Vertex v0 = {{}, c_out, {0,0}}, v1 = {{}, c_in, {1,0}}, v2 = {{}, c_in, {1,1}}, v3 = {{}, c_out, {0,1}};
        double circleStep = PI2/(float)circleverteces;
        for (int i = 0; i < circleverteces; i++)
        {
            float a = i*circleStep;
            v0.position = {x + math::lengthdir_x(r2, a), y + math::lengthdir_y(r2, a), 0.f};
            v1.position = {x + math::lengthdir_x(r1, a), y + math::lengthdir_y(r1, a), 0.f};
            v2.position = {x + math::lengthdir_x(r1, fmin(a2,a+circleStep)), y + math::lengthdir_y(r1, fmin(a2,a+circleStep)), 0.f};
            v3.position = {x + math::lengthdir_x(r2, fmin(a2,a+circleStep)), y + math::lengthdir_y(r2, fmin(a2,a+circleStep)), 0.f};
            engineData::layers[engineData::targetLayer]->getBatch()->draw(defaultTexture(),v0,v1,v2,v3,defaultBlendMode);
        }
    }

    //assumes the texture is vertically repeatable
    void draw_circle_arc_textured(int x, int y, float r1, float r2, float a1, float a2, int texID, float u1, float u2, int repetitions)
    {
        double circleStep = PI2/(float)circleverteces;
        float vStep = repetitions / (2 * PI / circleStep);
        Vertex v0 = {{}, defaultDrawColor, {u1,0}}, v1 = {{}, defaultDrawColor, {u2,0}},
               v2 = {{}, defaultDrawColor, {u2,vStep}}, v3 = {{}, defaultDrawColor, {u1,vStep}};
        int i = 0;
        for (double a = a1; a < a2; a += circleStep)
        {
            v0.position = {x + math::lengthdir_x(r2, a), y + math::lengthdir_y(r2, a), 0.f};
            v0.uv.v = i * vStep;
            v1.position = {x + math::lengthdir_x(r1, a), y + math::lengthdir_y(r1, a), 0.f};
            v1.uv.v = i * vStep;
            v2.position = {x + math::lengthdir_x(r1, fmin(a2,a+circleStep)), y + math::lengthdir_y(r1, fmin(a2,a+circleStep)), 0.f};
            v2.uv.v = (i+1) * vStep;
            v3.position = {x + math::lengthdir_x(r2, fmin(a2,a+circleStep)), y + math::lengthdir_y(r2, fmin(a2,a+circleStep)), 0.f};
            v3.uv.v = (i+1) * vStep;
            engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,v0,v1,v2,v3,defaultBlendMode);
            i++;
        }
    }

    glm::vec3 getCylPos(float r, float h, float a)
    {
        glm::vec3 pos;
        pos.z = h;
        pos.x = r * cos(a);
        pos.y = r * sin(a);
        return pos;
    }

    inline Position vec4ToPosition(glm::vec4 v) { return {v.x, v.y, v.z}; }

    void draw_cylinder(glm::vec3 pos, glm::vec3 rot, float r, float h, float a1, float a2, int texID, float u1, float u2, int repetitions)
    {
        double circleStep = abs(a2-a1)/(float)circleverteces;
        float vStep = repetitions / (float)circleverteces;
        Vertex v0 = {{}, defaultDrawColor, {u1,0}}, v1 = {{}, defaultDrawColor, {u2,0}},
               v2 = {{}, defaultDrawColor, {u2,vStep}}, v3 = {{}, defaultDrawColor, {u1,vStep}};
        int i = 0;
        glm::vec4 pos4 = glm::vec4(pos, 0);
        if (Inputs::Keyboard().Down(NSK_kp1)) rot.x += PI1_2;
        if (Inputs::Keyboard().Down(NSK_kp2)) rot.y += PI1_2;
        if (Inputs::Keyboard().Down(NSK_kp3)) rot.z += PI1_2;
        if (Inputs::Keyboard().Down(NSK_kp4)) rot.x += PI;
        if (Inputs::Keyboard().Down(NSK_kp5)) rot.y += PI;
        if (Inputs::Keyboard().Down(NSK_kp6)) rot.z += PI;
        if (Inputs::Keyboard().Down(NSK_kp7)) rot.x -= PI1_2;
        if (Inputs::Keyboard().Down(NSK_kp8)) rot.y -= PI1_2;
        if (Inputs::Keyboard().Down(NSK_kp9)) rot.z -= PI1_2;
        glm::mat4 rotation = glm::eulerAngleZYX(-rot.z, rot.y, rot.x);
        for (double a = a1; a < a2; a += circleStep)
        {
            float aa2 = fmin(a2,a+circleStep);
            v0.position = vec4ToPosition(pos4 + rotation * glm::vec4(getCylPos(r, -h/2.f, a),0));
            v0.uv.v = i * vStep;
            v1.position = vec4ToPosition(pos4 + rotation * glm::vec4(getCylPos(r, h/2.f, a),0));
            v1.uv.v = i * vStep;
            v2.position = vec4ToPosition(pos4 + rotation * glm::vec4(getCylPos(r, h/2.f, aa2),0));
            v2.uv.v = (i+1) * vStep;
            v3.position = vec4ToPosition(pos4 + rotation * glm::vec4(getCylPos(r, -h/2.f, aa2),0));
            v3.uv.v = (i+1) * vStep;
            engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,v0,v1,v2,v3,defaultBlendMode);
            i++;
        }
    }

    void draw_surface(int i, float x1, float y1, float x2, float y2, SpriteBatch* b, int bm)
    {
        if (b == nullptr) b = engineData::layers[engineData::targetLayer]->getBatch();
        b->draw(TextureManager::GetSurfaceTexID(i),
                {{x1,y1,0}, defaultDrawColor, {0,1}},
                {{x2,y1,0}, defaultDrawColor, {1,1}},
                {{x2,y2,0}, defaultDrawColor, {1,0}},
                {{x1,y2,0}, defaultDrawColor, {0,0}},
        bm);
    }

    void draw_line_3d(float x1, float y1, float z1, float x2, float y2, float z2, float width)
    {
        draw_line_color_3d(x1, y1, z1, x2, y2, z2, width, defaultDrawColor, defaultDrawColor);
    }

    void draw_line_color_3d(float x1, float y1, float z1, float x2, float y2, float z2, float width, Color c1, Color c2)
    {
        glm::vec3 pos = glm::vec3((x1+x2)/2, (y1+y2)/2, (z1+z2)/2);
        glm::vec3 axis = glm::vec3(x1-x2, y1-y2, z1-z2);
        glm::vec2 size = glm::vec2(width, sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2)));
        draw_billboard_axis(pos, axis, size, defaultTexture(), glm::vec4(0,0,1,1), c1, c1, c2, c2);
    }

    void draw_billboard(glm::vec3 pos, glm::vec2 size, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl)
    {
        glm::vec3 ViewX = glm::vec3(getViewMatrix()[0][0], getViewMatrix()[1][0], getViewMatrix()[2][0]);
        glm::vec3 ViewY = glm::vec3(getViewMatrix()[0][1], getViewMatrix()[1][1], getViewMatrix()[2][1]);
        glm::vec3 postl = pos - size.x/2 * ViewX + size.y/2 * ViewY;
        glm::vec3 postr = pos + size.x/2 * ViewX + size.y/2 * ViewY;
        glm::vec3 posbr = pos + size.x/2 * ViewX - size.y/2 * ViewY;
        glm::vec3 posbl = pos - size.x/2 * ViewX - size.y/2 * ViewY;
        engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,
                {posfromvec(postl), tl, {uvs.x,uvs.y}},
                {posfromvec(postr), tr, {uvs.z,uvs.y}},
                {posfromvec(posbr), br, {uvs.z,uvs.w}},
                {posfromvec(posbl), bl, {uvs.x,uvs.w}},
            defaultBlendMode);

    }

    void draw_billboard_axis(glm::vec3 pos, glm::vec3 axis, glm::vec2 size, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl)
    {
        axis = glm::normalize(axis);
        glm::vec3 frontvec;
        if (axis.x != 0 && axis.y != 0) frontvec = glm::normalize(glm::vec3(axis.y,-axis.x,0));
        else if (axis.x != 0 && axis.z != 0) frontvec = glm::normalize(glm::vec3(axis.z,0,-axis.x));
        else if (axis.z != 0 && axis.y != 0) frontvec = glm::normalize(glm::vec3(0,-axis.z,axis.y));
        else if (axis.x != 0) frontvec = glm::vec3(0,axis.x,0);
        else if (axis.y != 0) frontvec = glm::vec3(axis.y,0,0);
        else if (axis.z != 0) frontvec = glm::vec3(0,axis.z,0);
        glm::vec3 rightvec = glm::cross(axis, frontvec);
        glm::vec3 objtocam = glm::normalize(camPos() - pos);
        objtocam = glm::normalize(objtocam - (axis * glm::dot(objtocam,axis)));
        glm::vec3 axisRot = glm::cross(frontvec,objtocam);
        glm::vec4 p = glm::vec4(pos,0);
        float ang = acos(glm::dot(frontvec,objtocam));
        glm::mat4 rot = glm::rotate(glm::mat4(1), ang, axisRot);
        glm::vec4 postl = p + rot * glm::vec4(-size.x/2 * rightvec + size.y/2 * axis,0);
        glm::vec4 postr = p + rot * glm::vec4(size.x/2 * rightvec + size.y/2 * axis,0);
        glm::vec4 posbr = p + rot * glm::vec4(size.x/2 * rightvec - size.y/2 * axis,0);
        glm::vec4 posbl = p + rot * glm::vec4(-size.x/2 * rightvec - size.y/2 * axis,0);
        engineData::layers[engineData::targetLayer]->getBatch()->draw(texID,
                {posfromvec(postl), tl, {uvs.x,uvs.y}},
                {posfromvec(postr), tr, {uvs.z,uvs.y}},
                {posfromvec(posbr), br, {uvs.z,uvs.w}},
                {posfromvec(posbl), bl, {uvs.x,uvs.w}},
            defaultBlendMode);
    }

}
