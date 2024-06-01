#version 150

out vec4 color;

in vec2 texCoords;

uniform float aspect;
uniform float strength;
uniform float aberration;

uniform sampler2D tex;
uniform sampler2D tex2;

void main(void){

    vec2 disp = texture(tex2, texCoords).rg;
    disp = ((disp*2)-1) * vec2(strength, strength*aspect);
	color = texture(tex, texCoords+disp);

    // chromatic aberration
    //*

    color += texture(tex, texCoords+disp*(1+aberration)) * vec4(1.0, 0.0, 1.0, 0.0);
    color += texture(tex, texCoords+disp*(1-aberration)) * vec4(0.0, 1.0, 1.0, 0.0);
    color *= vec4(0.5, 0.5, 0.3333, 1.0);

    //*/

    // see surface
    //color = texture(tex2, texCoords);
}
