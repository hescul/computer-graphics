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
		const glm::vec3& baseCenter,
		const glm::vec3& baseCorner
	) : _baseCenter{ baseCenter }, _baseCorner{ baseCorner } {}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _baseCenter;

	const glm::vec3 _baseCorner;
};