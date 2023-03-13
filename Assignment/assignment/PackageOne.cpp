#include <cmath>
#include <numbers>

#include "PackageOne.h"
#include "../drawable/Drawable.h"
#include "../drawable/Color.h"

std::vector<float> BakedTriangle::vertices() const {
	return std::vector{
		// position				// color
		_p0.x, _p0.y, _p0.z,	srgb::RED[0],	srgb::RED[1],	srgb::RED[2],
		_p1.x, _p1.y, _p1.z,	srgb::GREEN[0], srgb::GREEN[1], srgb::GREEN[2],
		_p2.x, _p2.y, _p2.z,	srgb::BLUE[0],	srgb::BLUE[1],	srgb::BLUE[2],
	};
}

std::vector<Primitive> BakedTriangle::primitives() const {
	return std::vector{
		Primitive{ GL_TRIANGLES, std::vector{ 0u, 1u, 2u } }
	};
}

std::vector<float> BakedTetrahedron::vertices() const {
	return std::vector{
		// position				// color
		_p0.x, _p0.y, _p0.z,	srgb::RED[0],	srgb::RED[1],	srgb::RED[2],
		_p1.x, _p1.y, _p1.z,	srgb::GREEN[0], srgb::GREEN[1], srgb::GREEN[2],
		_p2.x, _p2.y, _p2.z,	srgb::BLUE[0],	srgb::BLUE[1],	srgb::BLUE[2],
		_p3.x, _p3.y, _p3.z,	srgb::CYAN[0],	srgb::CYAN[1],	srgb::CYAN[2],
	};
}

std::vector<Primitive> BakedTetrahedron::primitives() const {
	return std::vector{
		Primitive{ GL_TRIANGLE_STRIP, std::vector{ 0u, 2u, 1u, 3u, 0u } }
	};
}

std::vector<float> BakedCube::vertices() const {
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
		// position			// color
		p0.x, p0.y, p0.z,	srgb::RED[0],		srgb::RED[1],		srgb::RED[2],	
		p1.x, p1.y, p1.z,	srgb::BLACK[0],		srgb::BLACK[1],		srgb::BLACK[2],
		p2.x, p2.y, p2.z,	srgb::GREEN[0],		srgb::GREEN[1],		srgb::GREEN[2],
		p3.x, p3.y, p3.z,	srgb::YELLOW[0],	srgb::YELLOW[1],	srgb::YELLOW[2],
		p4.x, p4.y, p4.z,	srgb::MAGENTA[0],	srgb::MAGENTA[1],	srgb::MAGENTA[2],
		p5.x, p5.y, p5.z,	srgb::BLUE[0],		srgb::BLUE[1],		srgb::BLUE[2],
		p6.x, p6.y, p6.z,	srgb::CYAN[0],		srgb::CYAN[1],		srgb::CYAN[2],
		p7.x, p7.y, p7.z,	srgb::WHITE[0],		srgb::WHITE[1],		srgb::WHITE[2],
	};
}

std::vector<Primitive> BakedCube::primitives() const {
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

std::vector<float> BakedCone::vertices() const {
	auto vertices = std::vector{
		// base center
		_center.x, _center.y, _center.z,
		srgb::CYAN[0], srgb::CYAN[1], srgb::CYAN[2]
	};

	// Base circle
	for (auto i = 0; i < SEGMENTS; ++i) {
		const auto angle = static_cast<float>(i) * 2.0f * std::numbers::pi_v<float> / SEGMENTS;
		const auto rot = glm::vec3{ std::cos(angle), std::sin(angle), 0.0f };
		const auto dir = normalize(cross(_up, rot));
		const auto point = _center + dir * _radius;
		vertices.push_back(point.x);
		vertices.push_back(point.y);
		vertices.push_back(point.z);
		vertices.push_back(srgb::PURPLE[0]);
		vertices.push_back(srgb::PURPLE[1]);
		vertices.push_back(srgb::PURPLE[2]);
	}

	const auto top = _center + _up * _height;
	vertices.push_back(top.x);
	vertices.push_back(top.y);
	vertices.push_back(top.z);
	vertices.push_back(srgb::RED[0]);
	vertices.push_back(srgb::RED[1]);
	vertices.push_back(srgb::RED[2]);

	return vertices;
}

std::vector<Primitive> BakedCone::primitives() const {
	auto circleIndices = std::vector{ 0u };
	for (auto i = 0; i < SEGMENTS; ++i) {
		circleIndices.push_back(static_cast<IndexType>(i + 1));
	}
	circleIndices.push_back(1u);

	auto coneIndices = std::vector{ static_cast<IndexType>(SEGMENTS + 1) };
	for (auto i = 0; i < SEGMENTS; ++i) {
		coneIndices.push_back(static_cast<IndexType>(i + 1));
	}
	coneIndices.push_back(1u);

	return std::vector{
		Primitive{ GL_TRIANGLE_FAN, circleIndices },
		Primitive{ GL_TRIANGLE_FAN, coneIndices }
	};
}

std::vector<float> BakedStripSphere::vertices() const {
	auto vertices = std::vector<float>{};

	const auto top = _center + glm::vec3{ 0.0f, 0.0f, 1.0f } *_radius;
	vertices.push_back(top.x);
	vertices.push_back(top.y);
	vertices.push_back(top.z);
	vertices.push_back(srgb::YELLOW[0]);
	vertices.push_back(srgb::YELLOW[1]);
	vertices.push_back(srgb::YELLOW[2]);

	const auto bot = _center + glm::vec3{ 0.0f, 0.0f, -1.0f } *_radius;
	vertices.push_back(bot.x);
	vertices.push_back(bot.y);
	vertices.push_back(bot.z);
	vertices.push_back(srgb::YELLOW[0]);
	vertices.push_back(srgb::YELLOW[1]);
	vertices.push_back(srgb::YELLOW[2]);

	for (auto i = 1; i < DIVISIONS; ++i) {
		const auto theta = static_cast<float>(i) * std::numbers::pi_v<float> / DIVISIONS;
		for (auto j = 0; j < SEGMENTS; ++j) {
			const auto phi = static_cast<float>(j) * 2.0f * std::numbers::pi_v<float> / SEGMENTS;

			const auto diX = std::sin(theta) * std::cos(phi);
			const auto diY = std::sin(theta) * std::sin(phi);
			const auto diZ = std::cos(theta);

			const auto dir = normalize(glm::vec3{ diX, diY, diZ });
			const auto point = _center + dir * _radius;

			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(point.z);
			vertices.push_back(srgb::CYAN[0]);
			vertices.push_back(srgb::CYAN[1]);
			vertices.push_back(srgb::CYAN[2]);
		}
	}

	return vertices;
}

std::vector<Primitive> BakedStripSphere::primitives() const {
	auto primitives = std::vector<Primitive>{};

	for (auto i = 0; i < DIVISIONS - 2; ++i) {
		auto indices = std::vector<IndexType>{};
		for (auto j = 0; j < SEGMENTS; ++j) {
			indices.push_back(i * SEGMENTS + j + 2);
			indices.push_back((i + 1) * SEGMENTS + j + 2);
		}
		indices.push_back(i * SEGMENTS + 2);
		indices.push_back((i + 1) * SEGMENTS + 2);

		primitives.emplace_back(GL_TRIANGLE_STRIP, indices);
	}

	auto topIndices = std::vector{ 0u };
	for (auto i = 0; i < SEGMENTS; ++i) {
		topIndices.push_back(i + 2);
	}
	topIndices.push_back(2u);
	primitives.emplace_back(GL_TRIANGLE_FAN, topIndices);

	auto botIndices = std::vector{ 1u };
	constexpr auto lastDiv = DIVISIONS - 2;
	for (auto i = 0; i < SEGMENTS; ++i) {
		botIndices.push_back(lastDiv * SEGMENTS + i + 2);
	}
	botIndices.push_back(lastDiv * SEGMENTS + 2);
	primitives.emplace_back(GL_TRIANGLE_FAN, botIndices);

	return primitives;
}

std::vector<float> BakedCylinder::vertices() const {
	auto vertices = std::vector<float>{};

	for (auto i = 0; i < 2; ++i) {
		// the actual center
		const auto center = _center + _up * (_height * static_cast<float>(i));

		// center vertex
		vertices.push_back(center.x);
		vertices.push_back(center.y);
		vertices.push_back(center.z);
		vertices.push_back(srgb::BLUE[0]);
		vertices.push_back(srgb::BLUE[1]);
		vertices.push_back(srgb::BLUE[2]);
		// circular vertices
		for (auto j = 0; j < SEGMENTS; ++j) {
			// the angle of rotation
			const auto angle = static_cast<float>(j) * 2.0f * std::numbers::pi_v<float> / SEGMENTS;
			// rotation vector on the XY-plane
			const auto rot = glm::vec3{ std::cos(angle), std::sin(angle), 0.0f };
			// the direction to the point on circle
			const auto dir = normalize(cross(_up, rot));
			// translate to that point
			const auto point = center + dir * _radius;
			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(point.z);
			vertices.push_back(srgb::CYAN[0]);
			vertices.push_back(srgb::CYAN[1]);
			vertices.push_back(srgb::CYAN[2]);
		}
	}

	return vertices;
}

std::vector<Primitive> BakedCylinder::primitives() const {
	auto primitives = std::vector<Primitive>{};

	// the top and the bot triangle fans
	for (auto i = 0; i < 2; ++i) {
		auto baseIndices = std::vector{ static_cast<IndexType>(i * (SEGMENTS + 1)) };
		for (auto j = 0; j < SEGMENTS; ++j) {
			baseIndices.push_back(static_cast<IndexType>(j + 1 + i * (SEGMENTS + 1)));
		}
		baseIndices.push_back(1u + static_cast<IndexType>(i * (SEGMENTS + 1)));

		primitives.emplace_back(GL_TRIANGLE_FAN, baseIndices);
	}

	// the side triangle strip
	auto sideIndices = std::vector<IndexType>{};
	for (auto i = 0; i < SEGMENTS; ++i) {
		sideIndices.push_back(static_cast<IndexType>(i + 2 + SEGMENTS));
		sideIndices.push_back(static_cast<IndexType>(i + 1));
	}
	sideIndices.push_back(static_cast<IndexType>(2 + SEGMENTS));
	sideIndices.push_back(1u);
	primitives.emplace_back(GL_TRIANGLE_STRIP, sideIndices);

	return primitives;
}