#include "RenderableManager.h"

RenderableManager::Builder::Builder(const int count) {
	
}

RenderableManager::Builder& RenderableManager::Builder::shader(
	const int index, 
	const GLuint program
) {
	return *this;
}

