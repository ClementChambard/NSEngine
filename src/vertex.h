#ifndef VERTEXDOTH
#define VERTEXDOTH

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "color.h"

namespace NSEngine {

    struct Vertex {
        Vertex(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float u, float v)
        {
            position = glm::vec3(x, y, z);
            color = Color(r, g, b, a);
            uv = glm::vec2(u, v);
        }
        Vertex(const glm::vec3& p, const Color& c, const glm::vec2& u)
        {
            position = p;
            color = c;
            uv = u;
        }
        Vertex(const glm::vec3& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a, float u, float v)
        {
            position = p;
            color = Color(r, g, b, a);
            uv = glm::vec2(u, v);
        }
        Vertex(float x, float y, float z, const Color& c, float u, float v)
        {
            position = glm::vec3(x, y, z);
            color = c;
            uv = glm::vec2(u, v);
        }
        Vertex(float x, float y, float z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const glm::vec2& u)
        {
            position = glm::vec3(x, y, z);
            color = Color(r, g, b, a);
            uv = u;
        }
        Vertex(const glm::vec3& p, const Color& c, float u, float v)
        {
            position = p;
            color = c;
            uv = glm::vec2(u, v);
        }
        Vertex(const glm::vec3& p, uint8_t r, uint8_t g, uint8_t b, uint8_t a, const glm::vec2 u)
        {
            position = p;
            color = Color(r, g, b, a);
            uv = u;
        }
        Vertex(float x, float y, float z, const Color& c, const glm::vec2& u)
        {
            position = glm::vec3(x, y, z);
            color = c;
            uv = u;
        }
        Vertex() {}
        Vertex(const glm::vec3& p)
        {
            position = p;
        }
        Vertex(const Color& c)
        {
            color = c;
        }
        Vertex(const glm::vec2& u)
        {
            uv = u;
        }
        Vertex(const glm::vec3& p, const Color& c)
        {
            position = p;
            color = c;
        }
        Vertex(const Color& c, const glm::vec2& u)
        {
            color = c;
            uv = u;
        }
        Vertex(const glm::vec3& p, const glm::vec2& u)
        {
            uv = u;
            position = p;
        }
        glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
        Color color = Color(255, 255, 255, 255);
        glm::vec2 uv = glm::vec2(0.f, 0.f);
    };

    inline bool operator==(glm::vec3 p1, glm::vec3 p2) { return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z; }
    inline bool operator==(Color c1,Color c2) { return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b && c1.a == c2.a; }
    inline bool operator==(Vertex v1, Vertex v2) { return v1.position == v2.position && v1.color == v2.color && v1.uv == v2.uv; }
}

#endif
