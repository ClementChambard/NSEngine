#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "./defines.h"

namespace ns {

struct ColorRGB {
    ColorRGB() {}
    ColorRGB(glm::vec3 v) { r = v.r; g = v.g; b = v.b; }
    ColorRGB(u8 r, u8 g, u8 b)
    {
        this->r = (GLubyte)r;
        this->g = (GLubyte)g;
        this->b = (GLubyte)b;
    }
    GLubyte r = 255, g = 255, b = 255;
};

struct NSITPuint8_t {
    NSITPuint8_t() {}
    NSITPuint8_t(f32 i) { value = (u8) i; }
    NSITPuint8_t(u8 i) { value = (u8) i; }
    NSITPuint8_t(int i) { value = (u8) i; }
    operator f32() const { return (f32) value; }
    operator u8() const { return value; }
    bool operator==(f32 f) { return f == (f32)value; }
    u8 value = 0;
};

inline f32 operator-(NSITPuint8_t t1 ,NSITPuint8_t t2) { return (f32) t1 - (f32) t2; }
inline f32 operator*(f32 t1 ,NSITPuint8_t t2) { return (f32) t1 * (f32) t2; }
inline f32 operator+(NSITPuint8_t t1, f32 f) { return (f32) t1 + f; }
inline f32 operator+=(NSITPuint8_t t1, NSITPuint8_t t2) { return (t1 = t1+t2); }

struct Color {
    Color() {}
    Color(u8 r, u8 g, u8 b, u8 a)
    {
        this->r = (GLubyte)r;
        this->g = (GLubyte)g;
        this->b = (GLubyte)b;
        this->a = (GLubyte)a;
    }
    GLubyte r = 255, g = 255, b = 255, a = 255;
    void Darken(f32 x) { r /= x; g /= x; b /= x; }
};

inline glm::vec4 operator-(Color c1, Color c2) { return glm::vec4((f32)(c1.r-c2.r),(f32)(c1.g-c2.g),(f32)(c1.b-c2.b),(f32)(c1.a-c2.a)); }
inline Color operator+(Color c1, Color c2) { return Color(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b,c1.a+c2.a); }
inline Color operator+(Color c1, glm::vec4 c2) { return Color(c1.r+(u8)c2.r,c1.g+(u8)c2.g,c1.b+(u8)c2.b,c1.a+(u8)c2.a); }
inline Color operator+=(Color &c1, Color c2) { return (c1 = Color(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b,c1.a+c2.a)); }
inline Color operator*(f32 f, Color c2) { return Color((u8)(f*(f32)c2.r),(u8)(f*(f32)c2.g),(u8)(f*(f32)c2.b),(u8)(f*(f32)c2.a)); }
inline glm::vec3 operator-(ColorRGB c1, ColorRGB c2) { return glm::vec3((f32)(c1.r-c2.r),(f32)(c1.g-c2.g),(f32)(c1.b-c2.b)); }
inline ColorRGB operator+(ColorRGB c1, ColorRGB c2) { return ColorRGB(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b); }
inline ColorRGB operator+(ColorRGB c1, glm::vec3 c2) { return ColorRGB(c1.r+(u8)c2.r,c1.g+(u8)c2.g,c1.b+(u8)c2.b); }
inline ColorRGB operator+=(ColorRGB &c1, ColorRGB c2) { return (c1 = ColorRGB(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b)); }
inline ColorRGB operator*(f32 f, ColorRGB c2) { return ColorRGB((u8)(f*(f32)c2.r),(u8)(f*(f32)c2.g),(u8)(f*(f32)c2.b)); }
inline ColorRGB operator*(ColorRGB f, ColorRGB c2) { return ColorRGB((u8)((f32)f.r*(f32)c2.r),(u8)((f32)f.g*(f32)c2.g),(u8)((f32)f.b*(f32)c2.b)); }

inline Color mix(Color const& c1, Color const& c2) {
    Color out;
    int tmp;
    tmp = c1.r * c2.r / 255;
    if (tmp > 255) out.r = 255;
    else           out.r = tmp;
    tmp = c1.g * c2.g / 255;
    if (tmp > 255) out.g = 255;
    else           out.g = tmp;
    tmp = c1.b * c2.b / 255;
    if (tmp > 255) out.b = 255;
    else           out.b = tmp;
    tmp = c1.a * c2.a / 255;
    if (tmp > 255) out.a = 255;
    else           out.a = tmp;
    return out;
}

#define c_white ns::Color(255,255,255,255)
#define c_black ns::Color(0,0,0,255)
#define c_red ns::Color(255,0,0,255)
#define c_green ns::Color(0,255,0,255)
#define c_blue ns::Color(0,0,255,255)
#define c_cyan ns::Color(0,255,255,255)
#define c_yellow ns::Color(255,255,0,255)
#define c_hgray ns::Color(128,128,128,255)

}

#endif /* COLOR_H */
