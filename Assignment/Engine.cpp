#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <span>
#include <exception>
#include <memory>

#include "Engine.h"

std::unique_ptr<Engine> Engine::Factory::operator()(const Context& context) const {
	return std::unique_ptr<Engine>(new Engine{ context });
}

std::unique_ptr<Engine> Engine::create(const Context& context) {
	constexpr static auto FACTORY = Factory();
	return FACTORY(context);
}

Engine::Engine(const Context& context) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	context.registerFramebufferCallback([](const auto w, const auto h) {
		// make sure the viewport matches the new window dimensions
		// width and height will be significantly larger than specified on retina displays
		glViewport(0, 0, w, h);
	});
}

EntityManager* Engine::getEntityManager() const {
	return _entityManager;
}

Camera* Engine::createCamera(const Entity entity, const float initialRatio) {
	const auto camera = new Camera(entity, initialRatio);
	_cameras[entity] = camera;
	return camera;
}


void Engine::setClearColor(const float r, const float g, const float b, const float a) {
	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;
}

void Engine::setPolygonMode(const PolygonMode mode) {
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
}


Renderable Engine::loadMesh(const Drawable& drawable) {
	const auto vertices = drawable.vertices();
	const auto layout = drawable.layout();
	const auto primitives = drawable.primitives();

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	createVertexBuffer(vertices, layout);
	createIndexBuffer(primitives);

	glBindVertexArray(0);

	const auto renderable = static_cast<Renderable>(_meshes.size());
	_meshes.emplace_back(vao, drawable.shader, createElements(primitives));

	return renderable;
}

void Engine::createVertexBuffer(
	const std::vector<float>& vertices, 
	const std::vector<GenericAttribute>& layout
) {
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// We transfer the data down the GPU by mean of std::mem copy.

	// Create a dedicated memory region in the GPU.
	glBufferStorage(
		GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()),
		nullptr, GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT
	);

	// Acquire the pointer to the newly created memory.
	const auto vertexData = static_cast<float*>(glMapBufferRange(
		GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()),
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT
	));
	if (!vertexData) {
		throw std::exception("Could not map vertex buffer.");
	}

	// Transfer the vertices data.
	std::memcpy(vertexData, vertices.data(), sizeof(float) * vertices.size());

	// Finally we notify the GPU the change.
	glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	auto stride = 0;
	for (const auto [size, normalized] : layout) {
		stride += static_cast<int>(size);
	}
	stride *= sizeof(float);
	auto offset = 0;
	for (unsigned int idx = 0; idx < layout.size(); ++idx) {
		const auto [size, normalized] = layout[idx];
		glVertexAttribPointer(
			idx, static_cast<GLint>(size), GL_FLOAT, !normalized, stride,
			reinterpret_cast<void*>(offset * sizeof(GLfloat))  // NOLINT(performance-no-int-to-ptr)
		);
		glEnableVertexAttribArray(idx);
		offset += static_cast<int>(size);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_vertexBuffers.push_back(vbo);
}

void Engine::createIndexBuffer(const std::vector<Primitive>& primitives) {
	std::size_t size = 0;
	for (const auto& [_, indices] : primitives) {
		size += indices.size();
	}

	auto jointIndices = std::vector<IndexType>{};
	jointIndices.reserve(size);
	for (const auto& [_, indices] : primitives) {
		auto span = std::span(indices.data(), indices.size());
		jointIndices.insert(jointIndices.end(), span.begin(), span.end());
	}

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(IndexType) * jointIndices.size()),
		jointIndices.data(), GL_STATIC_DRAW
	);

	_indexBuffers.push_back(ibo);
}

std::vector<Element> Engine::createElements(const std::vector<Primitive>& primitives) {
	auto elements = std::vector<Element>{};
	auto offset = 0;

	for (const auto& [topology, indices] : primitives) {
		elements.emplace_back(topology, indices.size(), offset);
		offset += static_cast<int>(indices.size());
	}

	return elements;
}


void Engine::render(const Renderable renderable, const Camera& camera) const {
	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute MVP matrices
	const auto model = glm::mat4(1.0f);
	const auto view = camera.getViewMatrix();
	const auto projection = camera.getProjection();

	if (renderable < _meshes.size()) {
		// Specify which program to use first
		const auto program = _meshes[renderable].shader;
		glUseProgram(program);

		// Set the model matrix
		glUniformMatrix4fv(
			glGetUniformLocation(program, "model"),
			1, GL_FALSE, value_ptr(model)
		);

		// Set the view matrix
		glUniformMatrix4fv(
			glGetUniformLocation(program, "view"),
			1, GL_FALSE, value_ptr(view)
		);

		// Set the projection matrix
		glUniformMatrix4fv(
			glGetUniformLocation(program, "projection"), 
			1, GL_FALSE, value_ptr(projection)
		);

		// VAO will be linked to the currently used program
		glBindVertexArray(_meshes[renderable].vao);
		
		for (unsigned int i = 0; i < _meshes[renderable].elements.size(); ++i) {
			const auto [topology, count, offset, texture] = _meshes[renderable].elements[i];

			// Bind the texture before the draw call
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, texture);

			glDrawElements(
				topology, static_cast<GLsizei>(count), GL_UNSIGNED_INT,
				reinterpret_cast<void*>(offset * sizeof(GLuint)) // NOLINT(performance-no-int-to-ptr)
			);
		}
		
		glBindVertexArray(0);
	}
}

void Engine::destroyCamera(const Entity entity) {
	_cameras.erase(entity);
}

void Engine::destroy() {
	for (const auto& [vao, shader, elements] : _meshes) {
		glDeleteVertexArrays(1, &vao);
		glDeleteProgram(shader);
	}

	// destroy remaining vertex buffers
	for (const auto buffer : _vertexBuffers) {
		glDeleteBuffers(1, &buffer);
	}

	// destroy remaining index buffers
	for (const auto buffer : _indexBuffers) {
		glDeleteBuffers(1, &buffer);
	}

	// destroy remaining camera resources
	for (auto [_, ptr] : _cameras) {
		delete ptr;
	}

	// destroy entity manager
	delete _entityManager;
}


