#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string_view>
#include <vector>
#include <array>
#include "context.h"
#include "visual.h"

GLuint prepareShaderProgram();
std::vector<char> readFile(std::string_view);
void validateShaderCompilation(GLuint);

const auto vertices = std::array{
	0.5f,  0.5f, 0.0f,		// top right
	0.5f, -0.5f, 0.0f,		// bottom right
	-0.5f, -0.5f, 0.0f,		// bottom left
	-0.5f,  0.5f, 0.0f
};

const auto indices = std::array{
	0u, 1u, 3u,  // first Triangle
	1u, 2u, 3u   // second Triangle
};

int main() {
	auto context = Context("ModernOpenGL");
	context.bindKey(Context::Key::ESCAPE, [&]{ context.setClose(true); });

	const auto shaderProgram = prepareShaderProgram();

	const auto [vbo, ibo, vao] = createVisual(vertices, indices);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	context.loop(
	[&]{
			// state-setting function
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			// color buffer will be filled with the color configured by glClearColor
			// state-using function
			glClear(GL_COLOR_BUFFER_BIT);
			// set the active shader program gor subsequent drawing commands
			glUseProgram(shaderProgram);
			// bind the corresponding vao
			glBindVertexArray(vao);
			// draw using the index buffer
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			// optional: unbind vao
			glBindVertexArray(0);
		}
	);

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteProgram(shaderProgram);

	// clean up resources
	Context::terminate();

	return 0;
}


[[nodiscard]] GLuint prepareShaderProgram() {
	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	const auto vertexShaderCode = readFile("shaders/shader.vert");
	const auto vertexShaderSource = vertexShaderCode.data();
	const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	validateShaderCompilation(vertexShader);
	// fragment shader
	const auto fragmentShaderCode = readFile("shaders/shader.frag");
	const auto fragmentShaderSource = fragmentShaderCode.data();
	const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	validateShaderCompilation(fragmentShader);
	// link shaders to shader program: the final linked version of multiple shaders combined
	const auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}
	// once linked, the shaders are no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

[[nodiscard]] std::vector<char> readFile(const std::string_view fileName) {
	auto file = std::ifstream(fileName.data(), std::ios::ate);
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}
	const auto fileSize = static_cast<size_t>(file.tellg());
	auto buffer = std::vector<char>(fileSize);
	file.seekg(0);
	file.read(buffer.data(), static_cast<std::streamsize>(fileSize));
	file.close();
	return buffer;
}

void validateShaderCompilation(const GLuint shader) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
	}
}