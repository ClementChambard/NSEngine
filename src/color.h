#ifndef COLOR_H
#define COLOR_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>

namespace NSEngine {

    struct ColorRGB {
        ColorRGB() {}
        ColorRGB(glm::vec3 v) { r = v.r; g = v.g; b = v.b; }
        ColorRGB(glm::uint8_t r, uint8_t g, uint8_t b)
        {
            this->r = (GLubyte)r;
            this->g = (GLubyte)g;
            this->b = (GLubyte)b;
        }
        GLubyte r = 255, g = 255, b = 255;
    };

    struct NSITPuint8_t {
        NSITPuint8_t() {}
        NSITPuint8_t(float i) { value = (uint8_t) i; }
        NSITPuint8_t(uint8_t i) { value = (uint8_t) i; }
        NSITPuint8_t(int i) { value = (uint8_t) i; }
        operator float() const { return (float) value; }
        operator uint8_t() const { return value; }
        bool operator==(float f) { return f == (float)value; }
        uint8_t value = 0;
    };

    inline float operator-(NSITPuint8_t t1 ,NSITPuint8_t t2) { return (float) t1 - (float) t2; }
    inline float operator*(float t1 ,NSITPuint8_t t2) { return (float) t1 * (float) t2; }
    inline float operator+(NSITPuint8_t t1, float f) { return (float) t1 + f; }
    inline float operator+=(NSITPuint8_t t1, NSITPuint8_t t2) { return (t1 = t1+t2); }

    struct Color {
        Color() {}
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        {
            this->r = (GLubyte)r;
            this->g = (GLubyte)g;
            this->b = (GLubyte)b;
            this->a = (GLubyte)a;
        }
        GLubyte r = 255, g = 255, b = 255, a = 255;
        void Darken(float x) { r /= x; g /= x; b /= x; }
    };

    inline glm::vec4 operator-(Color c1, Color c2) { return glm::vec4((float)(c1.r-c2.r),(float)(c1.g-c2.g),(float)(c1.b-c2.b),(float)(c1.a-c2.a)); }
    inline Color operator+(Color c1, Color c2) { return Color(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b,c1.a+c2.a); }
    inline Color operator+(Color c1, glm::vec4 c2) { return Color(c1.r+(uint8_t)c2.r,c1.g+(uint8_t)c2.g,c1.b+(uint8_t)c2.b,c1.a+(uint8_t)c2.a); }
    inline Color operator+=(Color &c1, Color c2) { return (c1 = Color(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b,c1.a+c2.a)); }
    inline Color operator*(float f, Color c2) { return Color((uint8_t)(f*(float)c2.r),(uint8_t)(f*(float)c2.g),(uint8_t)(f*(float)c2.b),(uint8_t)(f*(float)c2.a)); }
    inline glm::vec3 operator-(ColorRGB c1, ColorRGB c2) { return glm::vec3((float)(c1.r-c2.r),(float)(c1.g-c2.g),(float)(c1.b-c2.b)); }
    inline ColorRGB operator+(ColorRGB c1, ColorRGB c2) { return ColorRGB(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b); }
    inline ColorRGB operator+(ColorRGB c1, glm::vec3 c2) { return ColorRGB(c1.r+(uint8_t)c2.r,c1.g+(uint8_t)c2.g,c1.b+(uint8_t)c2.b); }
    inline ColorRGB operator+=(ColorRGB &c1, ColorRGB c2) { return (c1 = ColorRGB(c1.r+c2.r,c1.g+c2.g,c1.b+c2.b)); }
    inline ColorRGB operator*(float f, ColorRGB c2) { return ColorRGB((uint8_t)(f*(float)c2.r),(uint8_t)(f*(float)c2.g),(uint8_t)(f*(float)c2.b)); }
    inline ColorRGB operator*(ColorRGB f, ColorRGB c2) { return ColorRGB((uint8_t)((float)f.r*(float)c2.r),(uint8_t)((float)f.g*(float)c2.g),(uint8_t)((float)f.b*(float)c2.b)); }
    inline std::ostream& operator<<(std::ostream& l, Color const& c) {
        return l << "{r:" << (int)c.r << ",g:" << (int)c.g << ",b:" << (int)c.b << ",a:" << (int)c.a << "}";
    }

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

    #define c_white NSEngine::Color(255,255,255,255)
    #define c_black NSEngine::Color(0,0,0,255)
    #define c_red NSEngine::Color(255,0,0,255)
    #define c_green NSEngine::Color(0,255,0,255)
    #define c_blue NSEngine::Color(0,0,255,255)
    #define c_cyan NSEngine::Color(0,255,255,255)
    #define c_yellow NSEngine::Color(255,255,0,255)
    #define c_hgray NSEngine::Color(128,128,128,255)

}

#endif /* COLOR_H */
