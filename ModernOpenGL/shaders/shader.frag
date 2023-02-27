#version 330 core

// fragment shader only requires a vec4 output variable that defines the final color output
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 