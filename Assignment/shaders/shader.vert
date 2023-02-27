#version 330 core

// declare all the input vertex attributes
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec4 aColor; // the color variable has attribute position 1

out vec4 vertexColor; // specify a color output to the fragment shader

void main() {
    // set the output of the vertex shader
    // in real applications the input data is usually not already in normalized device coordinates 
    // so we first have to transform the input data to coordinates that fall within OpenGL's visible region
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

    vertexColor = aColor;
}