#include "PackageOne.h"
#include "Drawable.h"
#include "Color.h"

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
	const auto radius = distance(_baseCenter, _baseCorner);

}

std::vector<Primitive> Cube::primitives() const {
	return std::vector{
		Primitive{GL_TRIANGLE_STRIP, std::vector{
			0u, 2u, 1u, 3u, 0u
		}}
	};
}
