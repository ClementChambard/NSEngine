#ifndef SHADERPROGRAM_HEADER_INCLUDED
#define SHADERPROGRAM_HEADER_INCLUDED

#include "./math/types/vec/vec2.hpp"
#include "./math/types/vec/vec3.hpp"
#include "./math/types/vec/vec4.hpp"
#include "./math/types/mat/mat4.hpp"
#include "./defines.h"

namespace ns {

/**
 * Class for a shader program (vertex + fragment).
 *
 * Used for binding a specific shader and setting uniforms.
 * Actual Shader programs sould be a new class with this one as a base.
 * (see bind_attributes and get_all_uniform_location virtual methods)
 *
 * For the best usage, create methods for setting uniforms using the load_* methods provided by
 * this class.
 */
class ShaderProgram {
public:
    /**
     * Constructor: loads the program from two files.
     * The constructor of a child class should call this constructor with the desired files.
     */
    ShaderProgram(cstr vertex_file, cstr fragment_file);
    
    virtual ~ShaderProgram();

    /**
     * Bind the program for drawing
     * program should be bound for setting uniforms
     */
    void use();

protected:

    /**
     * Should be overriden to bind all attribute names to a location
     * Call it in constructor.
     * See bind_attributes
     *
     * For correct use with DrawBatch, bind attributes in this order:
     * 0: position
     * 1: color
     * 2: uv
     */
    virtual void bind_attributes() {}

    /**
     * Should be overriden to get all uniform locations and store them inside a member variable
     * Call it in constructor.
     * See get_uniform_location
     */
    virtual void get_all_uniform_location() {}

    /**
     * Binds an attribute by its name to a location
     */
    void bind_attribute(u32 attribute, cstr variable_name);

    /**
     * Gets an uniform's location by its name
     */
    u32 get_uniform_location(cstr uniform_name);

    /**
     * Sets an uniform float value
     */
    void load_float(u32 location, f32 value);

    /**
     * Sets an uniform vec2 value
     */
    void load_vec2(u32 location, const vec2& value);

    /**
     * Sets an uniform vec3 value
     */
    void load_vec3(u32 location, const vec3& value);

    /**
     * Sets an uniform vec4 value
     */
    void load_vec4(u32 location, const vec4& value);

    /**
     * Sets an uniform mat4 value
     */
    void load_mat4(u32 location, const mat4& value);

    /**
     * Sets an uniform boolean value
     */
    void load_bool(u32 location, bool value) { load_float(location, (f32)value); }

    /**
     * Sets an uniform integer value
     */
    void load_int(u32 location, i32 value);

private:
    // OpenGL data    TODO: try to abstract it away
    u32 m_programID = 0;
    u32 m_vertexShaderID = 0;
    u32 m_fragmentShaderID = 0;
};

} // namespace ns

#endif // SHADERPROGRAM_HEADER_INCLUDED
