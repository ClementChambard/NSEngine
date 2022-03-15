#ifndef VERTEXDOTH
#define VERTEXDOTH

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace NSEngine {

    struct Position {
        Position() {}
        Position(glm::vec2 pos) { x = pos.x; y = pos.y; z =     0; }
        Position(glm::vec3 pos) { x = pos.x; y = pos.y; z = pos.z; }
        Position(glm::vec4 pos) { x = pos.x; y = pos.y; z = pos.z; }
        Position(float x, float y, float z)
        {
            this->x = (GLfloat)x;
            this->y = (GLfloat)y;
            this->z = (GLfloat)z;
        }
        GLfloat x = 0.f, y = 0.f, z = 0.f;
    };

    inline Position posfromvec(glm::vec2 v) { return Position(v.x,v.y,0.f); }
    inline Position posfromvec(glm::vec3 v) { return Position(v.x,v.y,v.z); }
    inline Position posfromvec(glm::vec4 v) { return Position(v.x,v.y,v.z); }

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


    #define c_white NSEngine::Color(255,255,255,255)
    #define c_black NSEngine::Color(0,0,0,255)
    #define c_red NSEngine::Color(255,0,0,255)
    #define c_green NSEngine::Color(0,255,0,255)
    #define c_blue NSEngine::Color(0,0,255,255)
    #define c_cyan NSEngine::Color(0,255,255,255)

    struct UV {
        UV() {}
        UV(float u, float v)
        {
            this->u = (GLfloat)u;
            this->v = (GLfloat)v;
        }
        GLfloat u = 0.f, v = 0.f;
    };


    struct Vertex {
        Vertex(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float u, float v)
        {
            position = Position(x, y, z);
            color = Color(r, g, b, a);
            uv = UV(u, v);
        }
        Vertex(const Position& p, const Color& c, const UV& u)
        {
            position = p;
            color = c;
            uv = u;
        }
        Vertex(const Position& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float u, float v)
        {
            position = p;
            color = Color(r, g, b, a);
            uv = UV(u, v);
        }
        Vertex(float x, float y, float z, const Color& c, float u, float v)
        {
            position = Position(x, y, z);
            color = c;
            uv = UV(u, v);
        }
        Vertex(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const UV& u)
        {
            position = Position(x, y, z);
            color = Color(r, g, b, a);
            uv = u;
        }
        Vertex(const Position& p, const Color& c, float u, float v)
        {
            position = p;
            color = c;
            uv = UV(u, v);
        }
        Vertex(const Position& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const UV u)
        {
            position = p;
            color = Color(r, g, b, a);
            uv = u;
        }
        Vertex(float x, float y, float z, const Color& c, const UV& u)
        {
            position = Position(x, y, z);
            color = c;
            uv = u;
        }
        Vertex() {}
        Vertex(const Position& p)
        {
            position = p;
        }
        Vertex(const Color& c)
        {
            color = c;
        }
        Vertex(const UV& u)
        {
            uv = u;
        }
        Vertex(const Position& p, const Color& c)
        {
            position = p;
            color = c;
        }
        Vertex(const Color& c, const UV& u)
        {
            color = c;
            uv = u;
        }
        Vertex(const Position& p, const UV& u)
        {
            uv = u;
            position = p;
        }
        Position position = Position(0.f, 0.f, 0.f);
        Color color = Color(255, 255, 255, 255);
        UV uv = UV(0.f, 0.f);
    };

    inline bool operator==(Position p1, Position p2) { return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z; }
    inline bool operator==(Color c1,Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a; }
    inline bool operator==(UV uv1, UV uv2) { return uv1.u == uv2.u && uv1.v == uv2.v; }
    inline bool operator==(Vertex v1, Vertex v2) { return v1.position == v2.position && v1.color == v2.color && v1.uv == v2.uv; }
}

#endif
