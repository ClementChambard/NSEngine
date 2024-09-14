#include "OBJLoader.h"
#include "Loader3D.h"
#include <logger.h>
#include <platform/filesystem.h>
#include <cstdlib>
#include <math/types/types.hpp>
#include <vector>
#include <string>

static bool string_begin_with(cstr s, cstr pattern) {
    char sc = *s;
    for (char c = *pattern; c; c = *(++pattern), sc = *(++s))
        if (sc != c) return false;
    return true;
}

static std::vector<std::string> string_split(std::string s, const char sep = ' ') {
    usize pos1 = 0;
    usize pos2 = 0;
    std::vector<std::string> strs;
    while (pos1 < s.size()) {
        for (pos2 = pos1; s[pos2] != sep && pos2 < s.size(); pos2++);
        strs.push_back(s.substr(pos1, pos2-pos1));
        pos1 = pos2+1;
    }
    return strs;
}


namespace ns::n3d::OBJLoader {

    void process_vertex(std::vector<std::string> vertex, std::vector<unsigned int>& indices, std::vector<vec2>& texcoords, std::vector<vec3>& normals, float* texcoords_ar, float* normals_ar);

    RawModel* load_obj_model(cstr file_name) {
        ns::fs::File f;
        if (!ns::fs::open(file_name, ns::fs::Mode::READ, false, &f))
        {
            NS_ERROR("Couldn't open OBJ file : %s", file_name);
        }

        std::vector<vec3> vertex_positions;
        std::vector<vec3> vertex_normals;
        std::vector<vec2> vertex_texcoords;
        std::vector<unsigned int> indices;
        float* positions = nullptr;
        float* normals = nullptr;
        float* texcoords = nullptr;

        pstr line = ns::alloc_n<char>(100);
        usize r;
        while (ns::fs::read_line(&f, 100, &line, &r)) {
            std::vector<std::string> current_line = string_split(line);
            if (string_begin_with(line, "v "))
            {
                vec3 vertexpos = vec3(std::stof(current_line[1]), std::stof(current_line[2]), std::stof(current_line[3]));
                vertex_positions.push_back(vertexpos);
            }
            else if (string_begin_with(line, "vt "))
            {
                vec2 vertextex = vec2(std::stof(current_line[1]), std::stof(current_line[2]));
                vertex_texcoords.push_back(vertextex);
            }
            else if (string_begin_with(line, "vn "))
            {
                vec3 vertexnor = vec3(std::stof(current_line[1]), std::stof(current_line[2]), std::stof(current_line[3]));
                vertex_normals.push_back(vertexnor);
            }
            else if (string_begin_with(line, "f "))
            {
                if (positions == nullptr)
                {
                    positions = new float[3*vertex_positions.size()];
                    normals = new float[3*vertex_positions.size()];
                    texcoords = new float[2*vertex_positions.size()];
                }
                std::vector<std::string> v1 = string_split(current_line[1], '/');
                std::vector<std::string> v2 = string_split(current_line[2], '/');
                std::vector<std::string> v3 = string_split(current_line[3], '/');
                process_vertex(v1, indices, vertex_texcoords, vertex_normals, texcoords, normals);
                process_vertex(v2, indices, vertex_texcoords, vertex_normals, texcoords, normals);
                process_vertex(v3, indices, vertex_texcoords, vertex_normals, texcoords, normals);
            }
        }
        ns::free_n(line, 100);
        ns::fs::close(&f);

        for (size_t i = 0; i < vertex_positions.size(); i++) {
            positions[i * 3    ] = vertex_positions[i].x;
            positions[i * 3 + 1] = vertex_positions[i].y;
            positions[i * 3 + 2] = vertex_positions[i].z;
        }

        unsigned int* indices_ar = new unsigned int[indices.size()];
        for (size_t i = 0; i < indices.size(); i++) indices_ar[i] = indices[i];

        return Loader3D::load_to_vao(positions, vertex_positions.size() * 3,
                                     texcoords, vertex_positions.size() * 2,
                                     normals,   vertex_positions.size() * 3,
                                     indices_ar, indices.size());
    }

    void process_vertex(std::vector<std::string> vertex, std::vector<unsigned int>& indices, std::vector<vec2>& texcoords,
            std::vector<vec3>& normals, float* texcoords_ar, float* normals_ar) {
        int current_vertex_pointer = std::stoi(vertex[0]) - 1;
        indices.push_back(current_vertex_pointer);

        vec2 current_tex = texcoords[std::stoi(vertex[1]) - 1];
        texcoords_ar[current_vertex_pointer*2    ] = current_tex.x;
        texcoords_ar[current_vertex_pointer*2 + 1] = current_tex.y;
     
        vec3 current_nor = normals[std::stoi(vertex[2]) - 1];
        normals_ar[current_vertex_pointer * 3    ] = current_nor.x;
        normals_ar[current_vertex_pointer * 3 + 1] = current_nor.y;
        normals_ar[current_vertex_pointer * 3 + 2] = current_nor.z;
    } 

}
