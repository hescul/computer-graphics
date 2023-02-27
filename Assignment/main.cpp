#include <glm/glm.hpp>
#include "Context.h"
#include "Engine.h"
#include "PackageOne.h"

int main() {
	auto context = Context("PackageOne");
	context.bindKey(Context::Key::ESCAPE, [&]{ context.setClose(true); });
	// context.bindKey(Context::Key::T, [&] { context.togglePolygonMode(); });
	context.bindKey(Context::Key::W, [&] { context.setPolygonMode(Context::PolygonMode::LINE); });
	context.bindKey(Context::Key::F, [&] { context.setPolygonMode(Context::PolygonMode::FILL); });

	auto engine = Engine();
	engine.setClearColor(0.09804f, 0.14118f, 0.15686f, 1.0f);
	
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
		glm::vec3{ 0.0f, -0.5f,  0.0f },	// base center
		0.5f,								// radius
		1.5f,								// height
		glm::vec3{ 0.0f,  1.0f,  0.5f }
	};

	const auto renderable = engine.loadMesh(cone);
	context.loop([&] { engine.render(renderable); });

	engine.destroy();
	Context::terminate();

	return 0;
}
