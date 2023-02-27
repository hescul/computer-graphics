#pragma once

#include <glad/glad.h>
#include <vector>
#include <string_view>
#include <array>
#include <memory>
#include "Mesh.h"
#include "Drawable.h"

using Renderable   = unsigned int;
using VertexBuffer = unsigned int;
using IndexBuffer  = unsigned int;

class Engine {
public:
	static [[nodiscard]] GLuint createShaderProgram(std::string_view vertexShaderUri, std::string_view fragmentShaderUri);

	void setClearColor(float r, float g, float b, float a);

	[[nodiscard]] Renderable loadMesh(const Drawable& drawable);

	void render(Renderable renderable) const;

	void destroy();

private:
	std::vector<Mesh> _meshes{};

	std::vector<std::unique_ptr<float[]>> _dataVertices;

	std::vector<std::unique_ptr<IndexType[]>> _dataIndices;

	std::vector<VertexBuffer> _vertexBuffers{};

	std::vector<IndexBuffer> _indexBuffers{};

	std::array<float, 4> _clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };

	static [[nodiscard]] std::vector<char> readShaderFile(std::string_view fileName);

	static void validateShaderCompilation(GLuint shader);

	[[nodiscard]] VertexBuffer createVertexBuffer(const std::vector<float>& vertices, const std::vector<GenericAttribute>& layout);

	[[nodiscard]] IndexBuffer createIndexBuffer(const std::vector<Primitive>& primitives);

	static [[nodiscard]] std::vector<Element> createElements(const std::vector<Primitive>& primitives);
};