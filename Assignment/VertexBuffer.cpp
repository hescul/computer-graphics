#include "VertexBuffer.h"

VertexBuffer::Builder& VertexBuffer::Builder::bufferCount(const int count) {
	_bufferCount = count;
	return *this;
}

VertexBuffer::Builder& VertexBuffer::Builder::vertexCount(const int count) {
	_vertexCount = count;
	return *this;
}

VertexBuffer::Builder& VertexBuffer::Builder::attribute(
	VertexAttribute attr, 
	int bufferIndex, 
	AttributeType type, 
	int byteOffset, 
	int byteStride
) {
	return *this;
}

VertexBuffer::Builder& VertexBuffer::Builder::normalize(const VertexAttribute attr) {
	switch (attr) {
	case VertexAttribute::POSITION: 
		_normalizePosition = true;
		break;
	case VertexAttribute::COLOR:
		_normalizeColor = true;
		break;
	case VertexAttribute::UV0:
		_normalizeUv0 = true;
		break;
	case VertexAttribute::UV1:
		_normalizeUv1 = true;
		break;
	}
	return *this;
}


VertexBuffer::VertexBuffer(bool normalizePosition, bool normalizeColor, bool normalizeUv0, bool normalizeUv1) {
	
}



