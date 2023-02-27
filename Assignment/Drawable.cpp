#include "Drawable.h"
#include "Vertex.h"
#include "Engine.h"

std::vector<GenericAttribute> BakedColorDrawable::layout() const {
	return std::vector{
		GenericAttribute{ AttributeSize::VEC_3, true },
		GenericAttribute{ AttributeSize::VEC_4, true }
	};
}

GLuint BakedColorDrawable::loadBakedColorShader() {
	return Engine::createShaderProgram(VERT_SHADER_PATH, FRAG_SHADER_PATH);
}


