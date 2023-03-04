// The main purpose of the fragment shader is to calculate the final color of a pixel 
// and this is usually the stage where all the advanced OpenGL effects occur. 
// Usually the fragment shader contains data about the 3D scene that it can use to calculate 
// the final pixel color (like lights, shadows, color of the light and so on).

#version 330 core

// fragment shader requires a vec4 output variable that defines the final color output
out vec4 FragColor;

// The input variable sent from the vertex shader.
// When the types and the names are equal on both sides, OpenGL will 
// link those variables together (done when linking a program object).
// Fragment interpolation will be applied to all the fragment shader's input attributes.
in vec4 vertColor;
in vec2 vertTexCoord;

// Another way to pass data from our application on the CPU to the shaders on the GPU.
// Uniforms are global in the sense that a uniform variable is unique per shader program object,
// and can be accessed from any shader at any stage in the shader program.
// Whatever you set the uniform value to, uniforms will keep their values until they're either 
// reset or updated.
// If a declared uniform isn't used anywhere in the GLSL code, the compiler will silently remove 
// the variable from the compiled version of the shader program.

// The fragment shader should also have access to the texture object, but how do we pass the texture 
// object to the fragment shader? GLSL has a built-in data-type for texture objects called a sampler 
// that takes as a postfix the texture type we want e.g. sampler1D, sampler3D or in our case sampler2D. 
// We can then add a texture to the fragment shader by simply declaring a uniform sampler2D 
// that we later assign our texture to.
uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
    // To sample the color of a texture we use GLSL's built-in texture function that takes as its first 
    // argument a texture sampler and as its second argument the corresponding texture coordinates.
    // GLSL's built-in mix function takes two values as input and linearly interpolates between them 
    // based on its third argument.
    FragColor = mix(texture(texture1, vertTexCoord), texture(texture2, vertTexCoord), 0.2);
} 