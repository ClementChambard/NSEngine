#version 130

in vec2 vertexPosition;
in vec2 vertexTexCoords;

out vec2 fragmentTexCoords;

uniform float attenuation;

void main()
{
    gl_Position = vec4(vertexPosition, attenuation, 1.f);

    fragmentTexCoords = vertexTexCoords;
}
