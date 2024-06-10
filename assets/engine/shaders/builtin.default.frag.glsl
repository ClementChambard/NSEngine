#version 130

in vec4 fragmentColor;
in vec2 fragmentTexCoords;

out vec4 color;

uniform sampler2D NSTexture;

void main() {
    vec4 col = texture(NSTexture, fragmentTexCoords);
    if (col.a == 0) discard;
    
    color = col * fragmentColor;
}
