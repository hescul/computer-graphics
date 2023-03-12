#include <vector>
#include <fstream>
#include <iostream>

#include "Shader.h"

GLuint Shader::createProgram(
	const std::string_view vertexShaderUri, 
	const std::string_view fragmentShaderUri
) {
	const auto vertexShaderCode = readShaderFile(vertexShaderUri);
	const auto vertexShaderSource = vertexShaderCode.data();
	const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	validateShaderCompilation(vertexShader);

	const auto fragmentShaderCode = readShaderFile(fragmentShaderUri);
	const auto fragmentShaderSource = fragmentShaderCode.data();
	const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	validateShaderCompilation(fragmentShader);

	const auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "SHADER: Linking Failed\n" << infoLog << '\n';
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}


std::vector<char> Shader::readShaderFile(const std::string_view uri) {
	auto file = std::ifstream(uri.data(), std::ios::ate);
	if (!file.is_open()) {
		throw std::runtime_error("SHADER: Failed to open file!");
	}
	const auto fileSize = static_cast<size_t>(file.tellg());
	auto buffer = std::vector<char>(fileSize);
	file.seekg(0);
	file.read(buffer.data(), static_cast<std::streamsize>(fileSize));
	file.close();
	return buffer;
}

void Shader::validateShaderCompilation(const GLuint shader) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "SHADER: Compilation Failed:\n" << infoLog << '\n';
	}
}

