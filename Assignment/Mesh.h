#pragma once

#include <glad/glad.h>

struct Element {
	const int topology;
	const std::size_t count;
	const int offset;
};

struct Mesh {
	const unsigned int vao;
	const GLuint shader;
	const std::vector<Element> elements;
};
