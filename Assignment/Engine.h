#pragma once

#include <glad/glad.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>

#include "Context.h"
#include "EntityManager.h"
#include "Camera.h"
#include "Mesh.h"
#include "drawable/Drawable.h"

using Renderable   = unsigned int;

class Engine {
public:
	enum class PolygonMode {
		LINE = GL_LINE,
		FILL = GL_FILL
	};

	static std::unique_ptr<Engine> create(const Context& context);

	[[nodiscard]] EntityManager* getEntityManager() const;

	Camera* createCamera(Entity entity, float initialRatio = 1.0f);

	void destroyCamera(Entity entity);

	void setClearColor(float r, float g, float b, float a);

	static void setPolygonMode(PolygonMode mode);

	[[nodiscard]] Renderable loadMesh(const Drawable& drawable);

	void render(Renderable renderable, const Camera& camera) const;

	void destroy();

private:
	explicit Engine(const Context& context);

	EntityManager* _entityManager{ EntityManager::get() };

	std::vector<Mesh> _meshes{};

	std::vector<GLuint> _vertexBuffers{};

	std::vector<GLuint> _indexBuffers{};

	std::array<float, 4> _clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };

	void createVertexBuffer(const std::vector<float>& vertices, const std::vector<GenericAttribute>& layout);

	void createIndexBuffer(const std::vector<Primitive>& primitives);

	static [[nodiscard]] std::vector<Element> createElements(const std::vector<Primitive>& primitives);

	std::unordered_map<Entity, Camera*> _cameras{};

	class Factory {
	public:
		std::unique_ptr<Engine> operator()(const Context& context) const;
	};
};
