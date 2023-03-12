#include "Drawable.h"
#include "Vertex.h"
#include "../Shader.h"

std::vector<GenericAttribute> BakedColorDrawable::layout() const {
	return std::vector{
		GenericAttribute{ AttributeSize::VEC_3, true },
		GenericAttribute{ AttributeSize::VEC_3, true }
	};
}

GLuint BakedColorDrawable::loadBakedColorShader() {
	return Shader::createProgram(VERT_SHADER_PATH, FRAG_SHADER_PATH);
}


