#version 130

in vec2 fragmentTexCoords;

out vec4 color;

uniform sampler2D NSTexture;
uniform float attenuation;

void main() {

    vec4 col = texture(NSTexture, fragmentTexCoords);
    col.rgb = mix(col.rgb, vec3(0.5f, 0.5f, 1.f), 1-attenuation);
    color = col;
}
