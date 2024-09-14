#include "Loader3D.h"
#include "logger.h"
#include <GL/glew.h>

namespace ns::n3d {

    std::vector<u32> Loader3D::created_vaos;
    std::vector<u32> Loader3D::created_vbos;
    std::vector<u32> Loader3D::created_texs;

    RawModel* Loader3D::load_to_vao(f32* positions,      u32 positions_l,
                                    f32* texture_coords, u32 texture_coords_l,
                                    f32* normals,        u32 normals_l,
                                    u32* indices,        u32 indices_l)
    {
        u32 vao_id = create_vao();
        bind_indices_buffer(indices, indices_l);
        store_data_in_attribute_list(0, 3, positions, positions_l);
        store_data_in_attribute_list(1, 2, texture_coords, texture_coords_l);
        store_data_in_attribute_list(2, 3, normals, normals_l);
        unbind_vao();
        delete[] positions;
        delete[] texture_coords;
        delete[] normals;
        delete[] indices;
        return new RawModel(vao_id, indices_l);
    }

    RawModel* Loader3D::load_to_vao(f32* positions, u32 positions_l, u32 dimensions)
    {
        u32 vao_id = create_vao();
        store_data_in_attribute_list(0, dimensions, positions, positions_l);
        unbind_vao();
        return new RawModel(vao_id, positions_l / dimensions);
    }

    RawModel* Loader3D::load_to_vao(ModelData* md)
    {
        u32 vao_id = create_vao();
        u32 ni = md->get_num_indices();
        u32 nv = md->get_num_vertices();
        bind_indices_buffer(md->get_indices(), ni);
        store_data_in_attribute_list(0, 3, md->get_vertices(), nv*3);
        store_data_in_attribute_list(1, 2, md->get_texture_coords(), nv*2);
        store_data_in_attribute_list(2, 3, md->get_normals(), nv*3);
        unbind_vao();
        delete md;
        return new RawModel(vao_id, ni);
    }


    u32 Loader3D::load_cube_map(const std::vector<cstr> images)
    {
        u32 tex_id = 0;
        NS_ERROR("Used ns::n3d::Loader3D::load_cube_map which is not implemented");
        (void)images;
        // glGenTextures(1, &tex_id);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
        // for (u32 i = 0; i < images.size(); i++)
        // {
        //     u32 w, h, n;
        //     bytes buf = ns::ImageLoader::imageToBuffer(images[i], w, h, n);
        //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0,
        //             GL_RGBA, GL_UNSIGNED_BYTE, buf);
        // }
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // createdTexs.push_back(tex_id);
        return tex_id;
    }

    u32 Loader3D::create_vao()
    {
        u32 vao_id;
        glGenVertexArrays(1, &vao_id);
        created_vaos.push_back(vao_id);
        glBindVertexArray(vao_id);
        return vao_id;
    }

    void Loader3D::cleanup()
    {
        for (GLuint vao : created_vaos)
            glDeleteVertexArrays(1, &vao);

        for (GLuint vbo : created_vbos)
            glDeleteBuffers(1, &vbo);

        for (GLuint tex : created_texs)
            glDeleteTextures(1, &tex);

    }

    void Loader3D::store_data_in_attribute_list(i32 attribute_number, u32 dimensions, f32* data, u32 data_l)
    {
        GLuint vbo_id;
        glGenBuffers(1, &vbo_id);
        created_vbos.push_back(vbo_id);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ARRAY_BUFFER, data_l*sizeof(f32), data, GL_STATIC_DRAW);
        glVertexAttribPointer(attribute_number, dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Loader3D::unbind_vao()
    {
        glBindVertexArray(0);
    }

    void Loader3D::bind_indices_buffer(u32* indices, u32 indices_l)
    {
        GLuint vbo_id;
        glGenBuffers(1, &vbo_id);
        created_vbos.push_back(vbo_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_l*sizeof(i32), indices, GL_STATIC_DRAW);
    }

}



