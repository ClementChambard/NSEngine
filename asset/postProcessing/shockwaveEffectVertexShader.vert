#version 150

in vec2 position;

out vec2 texCoords;

void main(void){
	gl_Position = vec4(position, 0.0, 1.0);
	texCoords = position * 0.5 + 0.5;
}
