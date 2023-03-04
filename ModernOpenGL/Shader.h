#pragma once

#include <string_view>
#include <glad/glad.h>
#include <vector>

class Shader {
public:
	explicit Shader(std::string_view vertPath, std::string_view fragPath);

	void use() const;
	void destroy() const;

	void setBool(std::string_view name, bool value) const;
	void setInt(std::string_view name, int value) const;
	void setFloat(std::string_view name, float value) const;

private:
	const GLuint _program;

	static [[nodiscard]] GLuint createShaderProgram(std::string_view vertUri, std::string_view fragUri);

	static [[nodiscard]] std::vector<char> readFile(std::string_view fileName);

	static void validateShaderCompilation(GLuint shader);
};