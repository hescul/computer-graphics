#include <glad/glad.h>

#include "Renderer.h"

void Renderer::render(View* const view) const {
	auto clearMask = GL_DEPTH_BUFFER_BIT;
	if (_clearOptions.clear) {
		glClearColor(
			_clearOptions.clearColor[0],
			_clearOptions.clearColor[1],
			_clearOptions.clearColor[2],
			_clearOptions.clearColor[3]
		);
		clearMask |= GL_COLOR_BUFFER_BIT;
	}
	glClear(clearMask);
}

void Renderer::setClearOptions(const ClearOptions& options) {
	_clearOptions = options;
}

Renderer::ClearOptions Renderer::getClearOptions() const {
	return _clearOptions;
}

