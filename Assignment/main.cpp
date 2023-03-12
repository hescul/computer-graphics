#include <glm/glm.hpp>

#include "Context.h"
#include "Engine.h"

#include "assignment/PackageOne.h"

int main() {
	auto context = Context::create("PackageOne<1952092>");

	context->bindKey(Context::Key::ESCAPE, [&context]{ context->setClose(true); });
	context->bindKey(Context::Key::W, [] { Engine::setPolygonMode(Engine::PolygonMode::LINE); });
	context->bindKey(Context::Key::F, [] { Engine::setPolygonMode(Engine::PolygonMode::FILL); });

	auto engine = Engine::create(*context);
	engine->setClearColor(0.09804f, 0.14118f, 0.15686f, 1.0f);

	const auto camera = engine->createCamera(EntityManager::get()->create(), context->getInitialRatio());

	context->registerFramebufferCallback([&camera](const auto w, const auto h) {
		const auto ratio = static_cast<float>(w) / static_cast<float>(h);
		camera->setProjection(45.0f, ratio, 0.1f, 100.0f);
	});

	context->setMouseScrollCallback([&camera](const auto offsetY) {
		camera->relativeZoom(offsetY);
	});

	context->setMouseDragPerpetualCallback([&camera](const auto offsetX, const auto offsetY) {
		camera->relativeDrag(offsetX, offsetY);
	});

	const auto triangle = Triangle{
		glm::vec3{ -0.5f, -0.5f, 0.0f },
		glm::vec3{  0.5f, -0.5f, 0.0f },
		glm::vec3{  0.0f,  0.5f, 0.0f }
	};

	const auto tetrahedron = Tetrahedron{
		glm::vec3{ -0.5f,  0.0f, 0.0f },
		glm::vec3{  0.0f, -0.5f, 0.5f },
		glm::vec3{  0.5f,  0.0f, 0.0f },
		glm::vec3{ -0.5f,  0.8f, 0.0f }
	};

	const auto cube = Cube{
		glm::vec3{ 0.0f,  0.0f,  0.0f },	// center
		glm::vec3{ 0.0f,  1.0f,  1.0f },	// up vector
		glm::vec3{ 1.0f,  0.0f,  0.0f },	// corner vector
		1.0f								// side length
	};

	const auto cone = Cone{
		glm::vec3{ 0.0f,  0.0f,  0.0f },	// base center
		1.0f,								// radius
		2.0f,								// height
		glm::vec3{ 0.0f,  0.0f,  1.0f }		// up vector
	};

	const auto sphere = Sphere{
		glm::vec3{ 0.0f,  0.0f,  0.0f },	// center
		0.5f								// radius
	};

	const auto renderable = engine->loadMesh(cube);
	context->loop([&] { engine->render(renderable, *camera); });

	engine->destroyCamera(camera->getEntity());
	engine->destroy();

	return 0;
}
