// The first part of the pipeline is the vertex shader that takes as input a single vertex. 
// The main purpose of the vertex shader is to transform 3D coordinates into different 3D coordinates 
// and the vertex shader allows us to do some basic processing on the vertex attributes.

#version 330 core

// declare all the input vertex attributes
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
// We need to tell each vertex which part of the texture it corresponds to. Each vertex 
// should thus have a texture coordinate associated with them that specifies what part 
// of the texture image to sample from. Fragment interpolation then does the rest for 
// the other fragments.
layout (location = 2) in vec2 aTexCoord;

// This color will be sent to the fragment shader.
out vec4 vertColor;
// And this texture coordinate also.
out vec2 vertTexCoord;

void main() {
    // set the output of the vertex shader
    // in real applications the input data is usually not already in normalized device coordinates 
    // so we first have to transform the input data to coordinates that fall within OpenGL's visible region
    // Thanks to swizzling, we can flexibly pass data this way.
    gl_Position = vec4(aPos, 1.0f);

    vertColor = vec4(aColor, 1.0f);

    vertTexCoord = aTexCoord;
}