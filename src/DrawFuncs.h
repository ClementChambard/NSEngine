#ifndef DRAWFUNCS
#define DRAWFUNCS

#include <iostream>
#include "SpriteManager.h"
#include "GameSprites.h"
#include <glm/glm.hpp>

namespace NSEngine {
 
    extern void draw_game_sprite(int sprite_index, int x, int y, float xs, float ys, float rot, Color ctl, Color ctr, Color cbr, Color cbl);

    /** draw_set_[color,alpha,blend](value)
     *
     * set the default value of [color,alpha,blend] for the draw functions 
     */
    extern void draw_set_color(Color c);
    extern void draw_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    extern void draw_set_color(uint8_t r, uint8_t g, uint8_t b);
    extern void draw_set_alpha(uint8_t a);
    extern void draw_set_blend(int bm);

    /** draw_sprite[scaled,color,rotated](sprite_index, frame, position, [scale,rotation,color])
     *
     * draws the frame frame of sprite sprite_index at position position with extra options
     */
    extern void draw_sprite(int sprite_index, int frame, int x, int y);
    extern void draw_sprite_scaled(int sprite_index, int frame, int x, int y, float xs, float ys);
    extern void draw_sprite_rotated(int sprite_index, int frame, int x, int y, float rot);
    extern void draw_sprite_scrotatled(int sprite_index, int frame ,int x, int y, float xs, float ys, float rot);
    extern void draw_sprite_color(int sprite_index, int frame, int x, int y, Color c);
    extern void draw_sprite_scalor(int sprite_index, int frame, int x, int y, float xs, float ys, Color c);
    extern void draw_sprite_rotalor(int sprite_index, int frame, int x, int y, float rot, Color c);
    extern void draw_sprite_scrotalor(int sprite_index, int frame, int x, int y, float xs, float ys, float rot, Color c);
    
    /** draw_text(font_index, text, x, y)
     *
     * draws text text at position (x,y) with font font_index
     */
    extern void draw_text(int font_index, const char* text, int x, int y);

    /** draw_line[color,3d](pos1, pos2, width, [color])
     *
     * draws line from pos1 to pos2 with width width [and color color]
     */
    extern void draw_line(int x1, int y1, int x2, int y2, int width);
    extern void draw_line_color(int x1, int y1, int x2, int y2, int width, Color c1, Color c2);
    extern void draw_line_3d(float x1, float y1, float z1, float x2, float y2, float z2, float width);
    extern void draw_line_color_3d(float x1, float y1, float z1, float x2, float y2, float z2, float width, Color c1, Color c2);

    /** draw_rectangle[color](pos1, pos2, [colors], outline)
     *
     * draws a 2D axis aligned rectangle from pos1 to pos2 [with colors colors]
     * \param outline: if enabled, draws only the outline of the rectangle. disabled by default
     */
    extern void draw_rectangle(float x1, float y1, float x2, float y2, bool outline = false);
    extern void draw_rectangle_color(float x1, float y1, float x2, float y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);

    /** draw_quad[color,2d](pos1, pos2, pos3, pos4 [colors], outline)
     *
     * draws a quad with points at pos[1-4] [with colors colors]
     * \param outline: if enabled, draws only the outline of the quad. disabled by default
     * 2d : the z component is forced to 0
     */
    extern void draw_quad(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, bool outline = false);
    extern void draw_quad_color(glm::vec3 tl, glm::vec3 tr, glm::vec3 br, glm::vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);
    extern void draw_quad_2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, bool outline = false);
    extern void draw_quad_color_2d(glm::vec2 tl, glm::vec2 tr, glm::vec2 br, glm::vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);
    extern void draw_quad_tex_2d(GLuint texID, glm::vec4 tl, glm::vec4 tr, glm::vec4 br, glm::vec4 bl, bool outline = false);
    
    extern void draw_triangle(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, bool outline = false);
    extern void draw_triangle_color(glm::vec3 t1, glm::vec3 t2, glm::vec3 t3, Color ct1, Color ct2, Color ct3, bool outline = false);

    // set the vertex count of the draw_circle functions
    extern void draw_circle_set_vertex_count(int verteces);

    /** draw_circle[arc,color,textured](center, radius, [radius2, angle1, angle2], [colors, textureData], outline)
     *
     * draws a circle or circle arc from center center and radius radius [for circle arc : use radius2 and angle range]
     * \param outline: if enabled, draws only the outline of the circle. disabled by default
     * textureData : Id of texture + u value of texture coords (assumes vertical tiling) and repetition of texture along the arc
     */
    extern void draw_circle(int x, int y, float r, bool outline = false);
    extern void draw_circle_color(int x, int y, float r, Color c_in, Color c_out, bool outline = false);
    extern void draw_circle_arc(int x, int y, float r1, float r2, float a1, float a2);
    extern void draw_circle_arc_color(int x, int y, float r1, float r2, float a1, float a2, Color c_in, Color c_out);
    extern void draw_circle_arc_textured(int x, int y, float r1, float r2, float a1, float a2, int texID, float u1, float u2, int repetitions);

    extern void draw_cylinder(glm::vec3 pos, glm::vec3 rot, float r, float h, float a1, float a2, int texID, float u1, float u2, int repetitions);

    /** draw_billboard[sized,axis](pos, [axis], scale, textureData, colors)
     *
     * draws a sperical[/cylindrical] billboard rectangle of scale scale and position pos with colors colors [around axis axis]
     * textureData : Id of texture + uvs
     */
    extern void draw_billboard(glm::vec3 pos, glm::vec2 scale, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl);
    extern void draw_billboard_sized(glm::vec3 pos, glm::vec2 scale, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl);
    extern void draw_billboard_axis(glm::vec3 pos, glm::vec3 axis, glm::vec2 scale, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl);
    extern void draw_billboard_sized_axis(glm::vec3 pos, glm::vec3 axis, glm::vec2 scale, int texID, glm::vec4 uvs, Color tl, Color tr, Color br, Color bl);

    /** draw_AA_box(pos1, pos2, color, outline)
     * 
     * draws a 3d axis aligned box from pos1 to pos2 with color color
     * \param outline: if enabled, draws only the outline of the box. disabled by default
     * \param shade:   if enabled, shades the sides for pseudo lighting effect
     */
    extern void draw_AA_box(glm::vec3 p1, glm::vec3 p2, Color c, bool outline = false, bool shade = true);

    // draws a surface as a rectangle in batch b
    extern void draw_surface(int i, float x1, float y1, float x2, float y2, SpriteBatch* b = nullptr, int blendmode = 3);


}

#endif
