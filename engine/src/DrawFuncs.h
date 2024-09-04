#ifndef DRAWFUNCS_HEADER_INCLUDED
#define DRAWFUNCS_HEADER_INCLUDED

#include "DrawBatch.h"
#include "./defines.h"

namespace ns {

/** draw_set_color(value)
 *
 * set the default value of color for the draw functions
 * @param c the new drawing color
 */
void draw_set_color(Color c);

/** draw_set_color(value_rgba)
 *
 * set the default value of color for the draw functions
 * @param r the red component of the color
 * @param g the green component of the color
 * @param b the blue component of the color
 * @param a the alpha component of the color
 */
void draw_set_color(u8 r, u8 g, u8 b, u8 a);

/** draw_set_color(value_rgb)
 *
 * set the default value of color for the draw functions
 * @param r the red component of the color
 * @param g the green component of the color
 * @param b the blue component of the color
 */
void draw_set_color(u8 r, u8 g, u8 b);

/** draw_set_alpha(value)
 *
 * set the default value of alpha for the draw functions
 * @param a the new alpha value
 */
void draw_set_alpha(u8 a);

/** draw_set_layer(layerID)
 *
 * set on which layer the next draws will be on
 * @param layerID the id of the layer
 */
void draw_set_layer(usize layerID);

/** draw_set_circle_vertex_count(vertices)
 *
 * set the number of vertices that are part of the outline of a circle
 * - default is 90
 * @param vertices the number of vertices
 */
void draw_set_circle_vertex_count(i32 vertices);

/** draw_line[color](pos1, pos2, width, [color])
 *
 * draws line from pos1 to pos2 with width width [and color color]
 */
void draw_line(i32 x1, i32 y1, i32 x2, i32 y2, i32 width);
void draw_line_color(i32 x1, i32 y1, i32 x2, i32 y2, i32 width, Color c1, Color c2);

void batch_draw_line(DrawBatch* batch, i32 x1, i32 y1, i32 x2, i32 y2, i32 width);
void batch_draw_line_color(DrawBatch* batch,i32 x1, i32 y1, i32 x2, i32 y2, i32 width, Color c1, Color c2);

/** draw_rectangle[color](pos1, pos2, [colors], outline)
 *
 * draws a 2D axis aligned rectangle from pos1 to pos2 [with colors colors]
 * \param outline: if enabled, draws only the outline of the rectangle. disabled by default
 */
void draw_rectangle(f32 x1, f32 y1, f32 x2, f32 y2, bool outline = false);
void draw_rectangle_color(f32 x1, f32 y1, f32 x2, f32 y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);
void draw_rectangle_rotated(f32 cx, f32 cy, f32 w, f32 h, f32 rotation, bool outline = false);
void draw_rectangle_rotated_color(f32 cx, f32 cy, f32 w, f32 h, f32 rotation, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);

void batch_draw_rectangle(DrawBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, bool outline = false);
void batch_draw_rectangle_color(DrawBatch* batch, f32 x1, f32 y1, f32 x2, f32 y2, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);
void batch_draw_rectangle_rotated(DrawBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, bool outline = false);
void batch_draw_rectangle_rotated_color(DrawBatch* batch, f32 cx, f32 cy, f32 w, f32 h, f32 rotation, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);

/** draw_quad[color,2d](pos1, pos2, pos3, pos4 [colors], outline)
 *
 * draws a quad with points at pos[1-4] [with colors colors]
 * \param outline: @deprecated if enabled, draws only the outline of the quad. disabled by default
 * 2d : the z component is forced to 0
 */
void draw_quad(vec3 tl, vec3 tr, vec3 br, vec3 bl, [[deprecated]] bool outline = false);
void draw_quad_color(vec3 tl, vec3 tr, vec3 br, vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, [[deprecated]] bool outline = false);
void draw_quad_2d(vec2 tl, vec2 tr, vec2 br, vec2 bl, bool outline = false);
void draw_quad_color_2d(vec2 tl, vec2 tr, vec2 br, vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);

void batch_draw_quad(DrawBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl, [[deprecated]] bool outline = false);
void batch_draw_quad_color(DrawBatch* batch, vec3 tl, vec3 tr, vec3 br, vec3 bl, Color ctl, Color ctr, Color cbr, Color cbl, [[deprecated]] bool outline = false);
void batch_draw_quad_2d(DrawBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, bool outline = false);
void batch_draw_quad_color_2d(DrawBatch* batch, vec2 tl, vec2 tr, vec2 br, vec2 bl, Color ctl, Color ctr, Color cbr, Color cbl, bool outline = false);

void draw_triangle(vec3 t1, vec3 t2, vec3 t3, bool outline = false);
void draw_triangle_color(vec3 t1, vec3 t2, vec3 t3, Color ct1, Color ct2, Color ct3, bool outline = false);

void batch_draw_triangle(DrawBatch* batch, vec3 t1, vec3 t2, vec3 t3, bool outline = false);
void batch_draw_triangle_color(DrawBatch* batch, vec3 t1, vec3 t2, vec3 t3, Color ct1, Color ct2, Color ct3, bool outline = false);

/** draw_circle[arc,color,textured](center, radius, [radius2, angle1, angle2], [colors, textureData], outline)
 *
 * draws a circle or circle arc from center center and radius radius [for circle arc : use radius2 and angle range]
 * \param outline: if enabled, draws only the outline of the circle. disabled by default
 * textureData : Id of texture + u value of texture coords (assumes vertical tiling) and repetition of texture along the arc
 */
void draw_circle(i32 x, i32 y, f32 r, bool outline = false);
void draw_circle_color(i32 x, i32 y, f32 r, Color c_in, Color c_out, bool outline = false);
void draw_circle_arc(i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2);
void draw_circle_arc_color(i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, Color c_in, Color c_out);

void batch_draw_circle(DrawBatch* batch, i32 x, i32 y, f32 r, bool outline = false);
void batch_draw_circle_color(DrawBatch* batch, i32 x, i32 y, f32 r, Color c_in, Color c_out, bool outline = false);
void batch_draw_circle_arc(DrawBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2);
void batch_draw_circle_arc_color(DrawBatch* batch, i32 x, i32 y, f32 r1, f32 r2, f32 a1, f32 a2, Color c_in, Color c_out);

void draw_cylinder(vec3 pos, vec3 rot, f32 r, f32 h, f32 a1, f32 a2, f32 u1, f32 u2, i32 repetitions);

void batch_draw_cylinder(DrawBatch* batch, vec3 pos, vec3 rot, f32 r, f32 h, f32 a1, f32 a2, f32 u1, f32 u2, i32 repetitions);


/** draw_AA_box(pos1, pos2, color, outline)
 * 
 * draws a 3d axis aligned box from pos1 to pos2 with color color
 * \param outline: if enabled, draws only the outline of the box. disabled by default
 * \param shade:   if enabled, shades the sides for pseudo lighting effect
 */
void draw_aa_box(vec3 p1, vec3 p2, Color c, [[deprecated]] bool outline = false, bool shade = true);
void batch_draw_aa_box(DrawBatch* batch, vec3 p1, vec3 p2, Color c, [[deprecated]] bool outline = false, bool shade = true);

} // namespace ns

#endif // DRAWFUNCS_HEADER_INCLUDED
