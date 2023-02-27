#pragma once

#include <glad/glad.h>
#include <vector>
#include <string_view>

class Engine {
public:
	GLuint prepareShaderProgram(std::string_view vertexShaderUri, std::string_view fragmentShaderUri);

private:
	std::vector<GLuint> _shaderPrograms{};

};