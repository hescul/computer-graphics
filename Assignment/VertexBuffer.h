#pragma once

#include "Engine.h"

class VertexBuffer {
public:
	enum class VertexAttribute {
		POSITION	= 0,
		COLOR		= 1,
		UV0			= 2,
		UV1			= 3,
	};

	enum class AttributeType {
		FLOAT3,
		FLOAT2,
	};

	class Builder {
	public:
		Builder& bufferCount(int count);

		Builder& vertexCount(int count);

		Builder& attribute(
			VertexAttribute attr,
			int bufferIndex,
			AttributeType type,
			int byteOffset,
			int byteStride
		);

		Builder& normalize(VertexAttribute attr);

	private:
		int _bufferCount{ 0 };

		int _vertexCount{ 0 };

		bool _normalizePosition{ false };

		bool _normalizeColor{ false };

		bool _normalizeUv0{ false };

		bool _normalizeUv1{ false };
	};

	void setBufferAt(const Engine& engine, int bufferIndex, float* data);

private:
	VertexBuffer(

		bool normalizePosition,
		bool normalizeColor,
		bool normalizeUv0,
		bool normalizeUv1
	);


};