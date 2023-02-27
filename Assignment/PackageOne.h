#pragma once

#include <glm/glm.hpp>
#include "Drawable.h"


class Triangle final : public BakedColorDrawable {
public:
	explicit Triangle(
		const glm::vec3& p0, 
		const glm::vec3& p1, 
		const glm::vec3& p2
	) : _p0{ p0 }, _p1{ p1 }, _p2{ p2 } {}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _p0;

	const glm::vec3 _p1;

	const glm::vec3 _p2;
};

class Tetrahedron final : public BakedColorDrawable {
public:
	explicit Tetrahedron(
		const glm::vec3& p0, 
		const glm::vec3& p1, 
		const glm::vec3& p2,
		const glm::vec3& p3
	) : _p0{ p0 }, _p1{ p1 }, _p2{ p2 }, _p3{ p3 } {}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _p0;

	const glm::vec3 _p1;

	const glm::vec3 _p2;

	const glm::vec3 _p3;
};

class Cube final : public BakedColorDrawable {
public:
	Cube(
		const glm::vec3& center,
		const glm::vec3& upDir,
		const glm::vec3& corDir,
		const float sideLength
	) : _center{ center }, _upDir{ normalize(upDir) }, _corDir{ normalize(corDir) }, _sideLength{ sideLength } {
		if (dot(upDir, corDir) != 0.0f) {
			throw std::exception{ "The up and corner directions of the cube are not perpendicular\n" };
		}

		if (sideLength <= 0.0f) {
			throw std::exception{ "The side length of the cube is not positive\n" };
		}
	}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _center;

	const glm::vec3 _upDir;

	const glm::vec3 _corDir;

	const float _sideLength;
};

