#pragma once

#include <glad/glad.h>

class RenderableManager {
	enum class PrimitiveType {
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP
	};

	class Builder {
	public:
		explicit Builder(int count);

		Builder& shader(int index, GLuint program);

	private:
		GLuint* _programs{ nullptr };

		
	};
};