#include "PackageOne.h"
#include "Drawable.h"
#include "Color.h"
#include <cmath>
#include <iostream>
#include <format>

std::vector<float> Triangle::vertices() const {
	return std::vector{
		_p0.x, _p0.y, _p0.z, srgb::RED[0], srgb::RED[1], srgb::RED[2], srgb::RED[3],
		_p1.x, _p1.y, _p1.z, srgb::GREEN[0], srgb::GREEN[1], srgb::GREEN[2], srgb::GREEN[3],
		_p2.x, _p2.y, _p2.z, srgb::BLUE[0], srgb::BLUE[1], srgb::BLUE[2], srgb::BLUE[3],
	};
}

std::vector<Primitive> Triangle::primitives() const {
	return std::vector{
		Primitive{ GL_TRIANGLES, std::vector{ 0u, 1u, 2u } }
	};
}

std::vector<float> Tetrahedron::vertices() const {
	return std::vector{
		_p0.x, _p0.y, _p0.z, srgb::RED[0], srgb::RED[1], srgb::RED[2], srgb::RED[3],
		_p1.x, _p1.y, _p1.z, srgb::GREEN[0], srgb::GREEN[1], srgb::GREEN[2], srgb::GREEN[3],
		_p2.x, _p2.y, _p2.z, srgb::BLUE[0], srgb::BLUE[1], srgb::BLUE[2], srgb::BLUE[3],
		_p3.x, _p3.y, _p3.z, srgb::CYAN[0], srgb::CYAN[1], srgb::CYAN[2], srgb::CYAN[3]
	};
}

std::vector<Primitive> Tetrahedron::primitives() const {
	return std::vector{
		Primitive{ GL_TRIANGLE_STRIP, std::vector{ 0u, 2u, 1u, 3u, 0u } }
	};
}

std::vector<float> Cube::vertices() const {
	const auto baseCenter = _center + -_upDir * _sideLength / 2.0f;
	const auto baseRadius = _sideLength / static_cast<float>(std::sqrt(2));

	const auto p0 = baseCenter + _corDir * baseRadius;
	const auto p1 = baseCenter + normalize(cross(_upDir, _corDir)) * baseRadius;
	const auto p2 = baseCenter + -_corDir * baseRadius;
	const auto p3 = baseCenter + normalize(cross(_corDir, _upDir)) * baseRadius;

	const auto p4 = p0 + _upDir * _sideLength;
	const auto p5 = p1 + _upDir * _sideLength;
	const auto p6 = p2 + _upDir * _sideLength;
	const auto p7 = p3 + _upDir * _sideLength;

	return std::vector{
		p0.x, p0.y, p0.z,	srgb::RED[0], srgb::RED[1], srgb::RED[2], srgb::RED[3],
		p1.x, p1.y, p1.z,	srgb::GREEN[0], srgb::GREEN[1], srgb::GREEN[2], srgb::GREEN[3],
		p2.x, p2.y, p2.z,	srgb::BLUE[0], srgb::BLUE[1], srgb::BLUE[2], srgb::BLUE[3],
		p3.x, p3.y, p3.z,	srgb::CYAN[0], srgb::CYAN[1], srgb::CYAN[2], srgb::CYAN[3],
		p4.x, p4.y, p4.z,	srgb::BROWN[0], srgb::BROWN[1], srgb::BROWN[2], srgb::BROWN[3],
		p5.x, p5.y, p5.z,	srgb::MAGENTA[0], srgb::MAGENTA[1], srgb::MAGENTA[2], srgb::MAGENTA[3],
		p6.x, p6.y, p6.z,	srgb::YELLOW[0], srgb::YELLOW[1], srgb::YELLOW[2], srgb::YELLOW[3],
		p7.x, p7.y, p7.z,	srgb::PURPLE[0], srgb::PURPLE[1], srgb::PURPLE[2], srgb::PURPLE[3],
	};
}

std::vector<Primitive> Cube::primitives() const {
	return std::vector{
		Primitive{GL_TRIANGLE_STRIP, std::vector{
			4u, 0u, 5u, 1u, 6u, 2u, 7u, 3u, 4u, 0u,		// sides
			// 0u, 4u,	// connection
			// 4u, 5u, 7u, 6u,	// top
			// 6u, 3u,	// connection
			// 3u, 2u, 0u, 1u,	// bottom
		}},
		Primitive{GL_TRIANGLE_STRIP, std::vector{
			4u, 5u, 7u, 6u,	// top
		}},
		Primitive{GL_TRIANGLE_STRIP, std::vector{
			3u, 2u, 0u, 1u,	// bottom
		}},
	};
}
