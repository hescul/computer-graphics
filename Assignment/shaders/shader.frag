#version 440 core

// fragment shader requires a vec4 output variable that defines the final color output
out vec4 FragColor;

// The input variable sent from the vertex shader.
// When the types and the names are equal on both sides, OpenGL will 
// link those variables together (done when linking a program object).
in vec4 vertexColor;

// Another way to pass data from our application on the CPU to the shaders on the GPU.
// Uniforms are global in the sense that a uniform variable is unique per shader program object,
// and can be accessed from any shader at any stage in the shader program.
// Whatever you set the uniform value to, uniforms will keep their values until they're either 
// reset or updated.
// If a declared uniform isn't used anywhere in the GLSL code, the compiler will silently remove 
// the variable from the compiled version of the shader program.
uniform vec4 ourColor;

void main() {
    FragColor = vertexColor;
} 