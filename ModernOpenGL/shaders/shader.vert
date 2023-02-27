#version 330 core

// declare all the input vertex attributes
layout (location = 0) in vec3 aPos;

void main() {
    // set the output of the vertex shader
    // in real applications the input data is usually not already in normalized device coordinates 
    // so we first have to transform the input data to coordinates that fall within OpenGL's visible region
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}