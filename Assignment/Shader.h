#pragma once

#include <glad/glad.h>
#include <vector>
#include <string_view>

class Shader {
public:
	static [[nodiscard]] GLuint createProgram(
		std::string_view vertexShaderUri,
		std::string_view fragmentShaderUri
	);

private:
	static [[nodiscard]] std::vector<char> readShaderFile(std::string_view uri);

	static void validateShaderCompilation(GLuint shader);
};