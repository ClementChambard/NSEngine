#include "OBJFileLoader.h"
#include "Vertex.h"
#include <cstdio>
#include <platform/filesystem.h>
#include <logger.h>
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


namespace ns::n3d::OBJFileLoader {

static void process_vertex(std::vector<std::string> vertex, std::vector<Vertex *> vertices, std::vector<u32> indices);
static void deal_with_already_processed_vertex(Vertex *previous_vertex, i32 new_texture, i32 new_normal, std::vector<u32> indices, std::vector<Vertex *> vertices);
static void remove_unused_vertices(std::vector<Vertex*> vertices);
static u32* convert_indices_to_array(std::vector<u32> indices, u32& nb);
static f32 convert_data_to_arrays(std::vector<Vertex*> vertices, std::vector<vec2> tex, std::vector<vec3> nor, f32* vert_ar, f32* tex_ar, f32* nor_ar);

ModelData* load_obj(cstr file_name) {
    ns::fs::File f;
    if (!ns::fs::open(file_name , ns::fs::Mode::READ, false, &f)) {
        NS_ERROR("Couldn't open OBJ file : %s", file_name);
        return nullptr;
    }
    NS_INFO("[N3D] Starting reading file %s", file_name);

    std::string line;
    std::vector<Vertex*> vertices;
    std::vector<vec2> texcoords;
    std::vector<vec3> normals;
    std::vector<u32> indices;

    usize len;
    pstr buf = ns::alloc_n<char>(100);
    while (ns::fs::read_line(&f, 100, &buf, &len)) {
        std::vector<std::string> currentLine = string_split(buf);
        if (string_begin_with(line.c_str(), "v ")) {
            vec3 vertexpos = vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
            vertices.push_back(new Vertex(vertices.size(), vertexpos));
        } else if (string_begin_with(line.c_str(), "vt ")) {
            vec2 vertextex = vec2(std::stof(currentLine[1]), std::stof(currentLine[2]));
            texcoords.push_back(vertextex);
        } else if (string_begin_with(line.c_str(), "vn ")) {
            vec3 vertexnor = vec3(std::stof(currentLine[1]), std::stof(currentLine[2]), std::stof(currentLine[3]));
            normals.push_back(vertexnor);
        } else if (string_begin_with(line.c_str(), "f ")) {
            std::vector<std::string> v1 = string_split(currentLine[1],'/');
            std::vector<std::string> v2 = string_split(currentLine[2],'/');
            std::vector<std::string> v3 = string_split(currentLine[3],'/');
            process_vertex(v1, vertices, indices);
            process_vertex(v2, vertices, indices);
            process_vertex(v3, vertices, indices);
        }
    }
    ns::free_n(buf, 100);
    ns::fs::close(&f);
    NS_INFO("[N3D] finished reading file.");
    remove_unused_vertices(vertices);
    f32* vertices_ar = new f32[vertices.size() * 3];
    f32* textures_ar = new f32[vertices.size() * 2];
    f32* normals_ar = new f32[vertices.size() * 3];
    f32 furthest_point = convert_data_to_arrays(vertices, texcoords, normals, vertices_ar, textures_ar, normals_ar);
    u32 num_id;
    u32* indices_ar = convert_indices_to_array(indices, num_id);
    ModelData* data = new ModelData(vertices_ar, textures_ar, normals_ar, indices_ar, furthest_point, vertices.size(), indices.size());
    NS_INFO("[N3D] model generated!");
    return data;
}

void process_vertex(std::vector<std::string> vertex, std::vector<Vertex*> vertices, std::vector<u32> indices) {
    u32 id = std::stoi(vertex[0]) -1;
    Vertex* current_vertex = vertices[id];
    i32 tid = std::stoi(vertex[1]) -1;
    i32 nid = std::stoi(vertex[2]) -1;
    if (!current_vertex->is_set()) {
        current_vertex->set_texture_index(tid);
        current_vertex->set_normal_index(nid);
    } else {
        deal_with_already_processed_vertex(current_vertex, tid, nid, indices, vertices);
    }
}

u32* convert_indices_to_array(std::vector<u32> indices, u32& nb) {
    u32* ar = new u32[indices.size()];
    for (usize i = 0; i < indices.size(); i++)
        ar[i] = indices[i];
    nb = indices.size();
    return ar;
}

f32 convert_data_to_arrays(std::vector<Vertex*> vertices, std::vector<vec2> tex, std::vector<vec3> nor, f32* vert_ar, f32* tex_ar, f32* nor_ar) {
    f32 furthest_point = 0;
    for (usize i = 0; i < vertices.size(); i++) {
        Vertex* current_vertex = vertices[i];
        if (current_vertex->get_length() > furthest_point)
            furthest_point = current_vertex->get_length();
        vec3 pos = current_vertex->get_position();
        vec2 texcoords = tex[current_vertex->get_texture_index()];
        vec3 normal = nor[current_vertex->get_normal_index()];
        vert_ar[i * 3    ] = pos.x;
        vert_ar[i * 3 + 1] = pos.y;
        vert_ar[i * 3 + 2] = pos.z;
        tex_ar[i * 2    ] = texcoords.x;
        tex_ar[i * 2 + 1] = texcoords.y;
        nor_ar[i * 3    ] = normal.x;
        nor_ar[i * 3 + 1] = normal.y;
        nor_ar[i * 3 + 2] = normal.z;
    }
    return furthest_point;
}

void deal_with_already_processed_vertex(Vertex* previous_vertex, i32 new_texture, i32 new_normal, std::vector<u32> indices, std::vector<Vertex*> vertices) {
    if (previous_vertex->has_same_texture_and_normal(new_texture, new_normal)) {
        indices.push_back(previous_vertex->get_index());
    } else {
        Vertex* another_vertex = previous_vertex->get_duplicate();
        if (another_vertex != nullptr) {
            deal_with_already_processed_vertex(another_vertex, new_texture, new_normal, indices, vertices);
        } else {
            Vertex* duplicate = new Vertex(vertices.size(), previous_vertex->get_position());
            duplicate->set_texture_index(new_texture);
            duplicate->set_normal_index(new_normal);
            previous_vertex->set_duplicate(duplicate);
            vertices.push_back(duplicate);
            indices.push_back(duplicate->get_index());
        }
    }
}

void remove_unused_vertices(std::vector<Vertex*> vertices) {
    for (Vertex* v : vertices) {
        if (!v->is_set()) {
            v->set_texture_index(0);
            v->set_normal_index(0);
        }
    }
}

}
