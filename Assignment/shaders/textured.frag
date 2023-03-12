#version 440 core

in vec2 vertTexCoord;

out vec4 FragColor;

uniform sampler2D texture;

void main() {
	FragColor = texture(texture, vertTexCoord);
}