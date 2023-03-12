#pragma once

#include <glad/glad.h>

struct Element {
	const int topology;
	const std::size_t count;
	const int offset;
	const GLuint texture;
};

struct Mesh {
	const GLuint vao;
	const GLuint shader;
	const std::vector<Element> elements;
};
