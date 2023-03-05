#include "Shader.h"
#include <fstream>
#include <iostream>

Shader::Shader(const std::string_view vertPath, const std::string_view fragPath)
	: _program{ createShaderProgram(vertPath, fragPath) } {}

GLuint Shader::createShaderProgram(const std::string_view vertUri, const std::string_view fragUri) {
    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    const auto vertexShaderCode = readFile(vertUri);
    const auto vertexShaderSource = vertexShaderCode.data();
    const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    validateShaderCompilation(vertexShader);
    // fragment shader
    const auto fragmentShaderCode = readFile(fragUri);
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


std::vector<char> Shader::readFile(const std::string_view fileName) {
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

void Shader::validateShaderCompilation(const GLuint shader) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
    }
}

void Shader::use() const {
    glUseProgram(_program);
}

void Shader::setBool(const std::string_view name, const bool value) const {
    // will return -1 if fail to resolve the uniform
    const auto location = glGetUniformLocation(_program, name.data());
    // Updating a uniform does require us to first use the program (by calling glUseProgram),
	// because it sets the uniform on the currently active shader program.
	// Since OpenGL is pure C, it does not support native function overloading,
	// hence we pick the overloaded function that corresponds with our type.
    glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string_view name, const int value) const {
    const auto location = glGetUniformLocation(_program, name.data());
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string_view name, const float value) const {
    const auto location = glGetUniformLocation(_program, name.data());
    glUniform1f(location, value);
}

void Shader::setTransform(const std::string_view name, const float* const value) const {
    const auto location = glGetUniformLocation(_program, name.data());
    // The second argument tells OpenGL how many matrices we'd like to send, which is 1.
    // The third argument asks us if we want to transpose our matrix, that is to swap the columns and rows.
    // OpenGL developers often use an internal matrix layout called column-major ordering which is the default
    // matrix layout in GLM so there is no need to transpose the matrices.
    glUniformMatrix4fv(location, 1, GL_FALSE, value);
}

void Shader::destroy() const {
    glDeleteProgram(_program);
}


