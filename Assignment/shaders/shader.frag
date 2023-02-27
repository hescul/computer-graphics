#version 330 core

// fragment shader only requires a vec4 output variable that defines the final color output
out vec4 FragColor;

in vec4 vertexColor; // the input variable from the vertex shader (same name and same type) 

void main() {
    FragColor = vertexColor;
} 