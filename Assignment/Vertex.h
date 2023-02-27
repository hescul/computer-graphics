#pragma once

enum class AttributeSize {
	VEC_1 = 1,
	VEC_2 = 2,
	VEC_3 = 3,
	VEC_4 = 4
};

struct GenericAttribute {
	const AttributeSize size;
	const bool normalized;
};
