
#include "View.h"

void View::setScene(Scene* const scene) {
	_scene = scene;
}

Scene* View::getScene() const {
	return _scene;
}

