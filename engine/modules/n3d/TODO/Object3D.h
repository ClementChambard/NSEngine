#ifndef OBJECT3D_HEADER_INCLUDED
#define OBJECT3D_HEADER_INCLUDED

#include "TexturedModel.h"
#include <defines.h>
#include <math/types/types.hpp>

namespace ns::n3d {

class Object3D {
    public:

        Object3D() {}
        Object3D(TexturedModel* m, const vec3& p, const vec3& r, const vec3& s) :
            model(m), position(p), rotation(r), scale(s) {}
        Object3D(TexturedModel* m, const i32 i, const vec3& p, const vec3& r, const vec3& s) :
            model(m), position(p), rotation(r), scale(s), texture_index(i) {}

        f32 get_texture_xoffset() const { return (f32)(texture_index % model->get_model_texture()->get_number_of_rows())/(f32)model->get_model_texture()->get_number_of_rows();}
        f32 get_texture_yoffset() const { return (f32)((f32)texture_index / model->get_model_texture()->get_number_of_rows())/(f32)model->get_model_texture()->get_number_of_rows();}

        TexturedModel* get_model() const { return model; }
        vec3 get_position() const { return position; }
        vec3 get_rotation() const { return rotation; }
        vec3 get_scale() const { return scale; }
        mat4 get_matrix() const { return mat4::mk_translate(position) * mat4::mk_euler_xyz(rotation) * mat4::mk_scale(scale); }
        i32 get_texture_index() const { return texture_index; }

        void set_position(const vec3& pos) { position = pos; }
        void inc_position(const vec3& pos) { position += pos; }
        void set_rotation(const vec3& rot) { rotation = rot; }
        void inc_rotation(const vec3& rot) { rotation += rot; }
        void set_scale(const vec3& sca) { scale = sca; }
        void inc_scale(const vec3& sca) { scale += sca; }
        void set_texture_index(const i32 i) { texture_index = i; }
        void set_textured_model(TexturedModel* m) { model = m; }

    private:
        TexturedModel* model = nullptr;
        vec3 position = {0, 0, 0};
        vec3 rotation = {0, 0, 0};
        vec3 scale = {1, 1, 1};
        i32 texture_index = 0;
};

}

#endif // OBJECT3D_HEADER_INCLUDED
