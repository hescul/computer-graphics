#include "Context.h"
#include "Engine.h"

#include "assignment/PackageOne.h"

int main() {
	auto context = Context::create("PackageOne<1952092>");

	context->bindKey(Context::Key::ESC, [&context]{ context->setClose(true); });
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

	const auto bakedTriangle = BakedTriangle();
	const auto bakedTetrahedron = BakedTetrahedron();
	const auto bakedCube = BakedCube();
	const auto bakedCone = BakedCone();
	const auto bakedStripSphere = BakedStripSphere();
	const auto bakedCylinder = BakedCylinder();

	const auto renderable = engine->loadMesh(bakedCylinder);

	context->loop([&] { engine->render(renderable, *camera); });

	engine->destroyCamera(camera->getEntity());
	engine->destroy();

	return 0;
}
