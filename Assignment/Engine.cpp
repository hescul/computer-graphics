#include "Engine.h"
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <algorithm>
#include <span>
#include <memory>

GLuint Engine::createShaderProgram(const std::string_view vertexShaderUri, const std::string_view fragmentShaderUri) {
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
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	return shaderProgram;
}

std::vector<char> Engine::readShaderFile(const std::string_view fileName) {
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

void Engine::validateShaderCompilation(const GLuint shader) {
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << '\n';
	}
}

void Engine::setClearColor(const float r, const float g, const float b, const float a) {
	_clearColor[0] = r;
	_clearColor[1] = g;
	_clearColor[2] = b;
	_clearColor[3] = a;
}

Renderable Engine::loadMesh(const Drawable& drawable) {
	const auto vertices = drawable.vertices();
	const auto layout = drawable.layout();
	const auto primitives = drawable.primitives();
	
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const auto vbo = createVertexBuffer(vertices, layout);
	const auto ibo = createIndexBuffer(primitives);
	glBindVertexArray(0);

	_vertexBuffers.push_back(vbo);
	_indexBuffers.push_back(ibo);

	const auto renderable = static_cast<Renderable>(_meshes.size());
	_meshes.push_back(Mesh{ vao, drawable.shader, createElements(primitives) });

	return renderable;
}

VertexBuffer Engine::createVertexBuffer(
	const std::vector<float>& vertices, 
	const std::vector<GenericAttribute>& layout
) {
	// Save the underlying data of the vertices vector
	_dataVertices.emplace_back(std::make_unique<float[]>(vertices.size()));
	std::ranges::copy(vertices.begin(), vertices.end(), _dataVertices.back().get());

	VertexBuffer vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(
		GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(float) * vertices.size()),
		_dataVertices.back().get(), GL_STATIC_DRAW
	);
	auto stride = 0;
	for (const auto& [size, normalized] : layout) {
		stride += static_cast<int>(size);
	}
	stride *= sizeof(float);
	auto offset = 0;
	for (unsigned int idx = 0; idx < layout.size(); ++idx) {
		const auto [size, normalized] = layout[idx];
		glVertexAttribPointer(
			idx, static_cast<GLint>(size), GL_FLOAT, !normalized, stride,
			reinterpret_cast<void*>(offset)  // NOLINT(performance-no-int-to-ptr)
		);
		glEnableVertexAttribArray(idx);
		offset += static_cast<int>(size) * static_cast<int>(sizeof(float));
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

IndexBuffer Engine::createIndexBuffer(const std::vector<Primitive>& primitives) {
	std::size_t size = 0;
	std::ranges::for_each(primitives.begin(), primitives.end(), 
		[&](const Primitive& primitive) { size += primitive.indices.size(); }
	);

	auto indices = std::vector<IndexType>{};
	indices.reserve(size);
	std::ranges::for_each(primitives.begin(), primitives.end(),[&](const Primitive& primitive) {
		auto span = std::span(primitive.indices.data(), primitive.indices.size());
		indices.insert(indices.end(), span.begin(), span.end());
	});

	_dataIndices.emplace_back(std::make_unique<IndexType[]>(indices.size()));
	std::ranges::copy(indices.begin(), indices.end(), _dataIndices.back().get());

	IndexBuffer ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(IndexType) * indices.size()),
		_dataIndices.back().get(), GL_STATIC_DRAW
	);

	return ibo;
}

std::vector<Element> Engine::createElements(const std::vector<Primitive>& primitives) {
	auto elements = std::vector<Element>{};
	auto offset = 0;

	std::ranges::for_each(primitives.begin(), primitives.end(), [&](const Primitive& primitive) {
		const auto element = Element{ primitive.topology, primitive.indices.size(), offset };
		elements.push_back(element);
		offset += static_cast<int>(primitive.indices.size());
	});

	return elements;
}


void Engine::render(const Renderable renderable) const {
	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (renderable < _meshes.size()) {
		glUseProgram(_meshes[renderable].shader);
		glBindVertexArray(_meshes[renderable].vao);

		const auto& elements = _meshes[renderable].elements;
		std::ranges::for_each(elements.begin(), elements.end(), [](const Element& element) {
			const auto [topology, count, offset] = element;
			glDrawElements(
				topology, static_cast<GLsizei>(count), 
				GL_UNSIGNED_INT, reinterpret_cast<void*>(offset) // NOLINT(performance-no-int-to-ptr)
			);
		});
		
		glBindVertexArray(0);
	}
}

void Engine::destroy() {
	std::ranges::for_each(_meshes.begin(), _meshes.end(),[](const Mesh& mesh) {
		glDeleteVertexArrays(1, &mesh.vao);
		glDeleteProgram(mesh.shader);
	});

	std::ranges::for_each(_vertexBuffers.begin(), _vertexBuffers.end(),[](const VertexBuffer buffer) {
		glDeleteBuffers(1, &buffer);
	});

	std::ranges::for_each(_indexBuffers.begin(), _indexBuffers.end(),[](const IndexBuffer buffer) {
		glDeleteBuffers(1, &buffer);
	});
}


