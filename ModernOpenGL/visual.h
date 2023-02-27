#pragma once

#include <array>
#include <glad/glad.h>

struct Visual {
	unsigned int vbo;
	unsigned int ibo;
	unsigned int vao;
};

template <std::size_t VertSize, std::size_t IdxSize>
[[nodiscard]] Visual createVisual(const std::array<float, VertSize>& vertices, const std::array<unsigned int, IdxSize>& indices);

template <std::size_t VertSize, std::size_t IdxSize>
[[nodiscard]] Visual createVisual(const std::array<float, VertSize>& vertices, const std::array<unsigned int, IdxSize>& indices) {
	// can be bound just like a vertex buffer object
	// any subsequent vertex attribute call from this point on will be stored inside the VAO
	// VAO makes switching between different vertex data and attribute configurations
	// as easy as binding a different VAO
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	// manage the memory of GPU that stores a large number of vertices
	// vertex shader will processes as much vertices as we tell it to from this memory which is extremely fast to access
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	// several buffers can be bound at once as long as they a different buffer type
	// from this point on any calls made to the GL_ARRAY_BUFFER will affect the vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// copy the vertex data to vbo
	// GL_STATIC_DRAW: the data is set only once and used many times
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	// tell OpenGL how it should interpret the vertex data
	// vertex attribute takes data from the currently bound vbo
	// param 1: which vertex attribute we want to configure
	// param 2: the number of values of this vertex attribute (vec3 -> 3)
	// param 3: the type of the data of this attribute values
	// param 4: should the data be normalized (for int, byte data)
	// param 5: the stride, i.e. the size of this vertex attribute
	// a stride of 0 will let OpenGL determine the size (only works when values are tightly packed)
	// param 6: the offset of where the attribute data begins in the buffer, i.e. the accumulative size of all attributes
	// preceding this attribute.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	// vertex attributes are disabled by default
	glEnableVertexAttribArray(0);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's
	// bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
	// do NOT unbind the IBO while a VAO is active as the bound element buffer object IS stored in the VAO
	// keep the IBO bound.

	// unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
	// Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs
	// (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return Visual{ vbo, ibo, vao };
}
