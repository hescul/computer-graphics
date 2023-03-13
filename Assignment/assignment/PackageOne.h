#pragma once

#include <glm/glm.hpp>

#include "../drawable/Drawable.h"


class BakedTriangle final : public BakedColorDrawable {
public:
	explicit BakedTriangle(
		const glm::vec3& p0 = glm::vec3{ -0.5f, -0.5f, 0.0f },
		const glm::vec3& p1 = glm::vec3{ 0.5f, -0.5f, 0.0f },
		const glm::vec3& p2 = glm::vec3{ 0.0f,  0.5f, 0.0f }
	) : _p0{ p0 }, _p1{ p1 }, _p2{ p2 } {}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _p0;

	const glm::vec3 _p1;

	const glm::vec3 _p2;
};

class BakedTetrahedron final : public BakedColorDrawable {
public:
	explicit BakedTetrahedron(
		const glm::vec3& p0 = glm::vec3{ -0.5f,  0.0f, 0.0f }, 
		const glm::vec3& p1 = glm::vec3{  0.0f, -0.5f, 0.5f }, 
		const glm::vec3& p2 = glm::vec3{  0.5f,  0.0f, 0.0f },
		const glm::vec3& p3 = glm::vec3{ -0.5f,  0.8f, 0.0f }
	) : _p0{ p0 }, _p1{ p1 }, _p2{ p2 }, _p3{ p3 } {}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _p0;

	const glm::vec3 _p1;

	const glm::vec3 _p2;

	const glm::vec3 _p3;
};

class BakedCube final : public BakedColorDrawable {
public:
	explicit BakedCube(
		const glm::vec3& center = glm::vec3{ 0.0f,  0.0f,  0.0f },
		const glm::vec3& upDir  = glm::vec3{ 0.0f,  0.0f,  1.0f },
		const glm::vec3& corDir = glm::vec3{ 1.0f,  0.0f,  0.0f },
		const float sideLength  = 1.0f
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

class BakedCone final : public BakedColorDrawable {
public:
	explicit BakedCone(
		const glm::vec3& center = glm::vec3{ 0.0f,  0.0f, -1.0f },
		const float radius = 1.0f,
		const float height = 2.0f,
		const glm::vec3& up = glm::vec3{ 0.0f,  0.0f, 1.0f }
	) : _center{ center }, _radius{ radius }, _height{ height }, _up{ normalize(up) } {
		if (height <= 0.0f) {
			throw std::exception{ "The height of the cone is not positive\n" };
		}

		if (radius <= 0.0f) {
			throw std::exception{ "The radius of the cone is not positive\n" };
		}
	}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _center;

	const float _radius;

	const float _height;

	const glm::vec3 _up;

	static constexpr auto SEGMENTS = 100;
};

class BakedStripSphere final : public BakedColorDrawable {
public:
	explicit BakedStripSphere(
		const glm::vec3& center = glm::vec3{ 0.0f, 0.0f, 0.0f },
		const float radius = 1.0f
	) : _center{ center }, _radius{ radius } {
		if (radius <= 0.0f) {
			throw std::exception{ "The radius of the sphere is not positive\n" };
		}
	}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _center;

	const float _radius;

	static constexpr auto SEGMENTS = 50;

	static constexpr auto DIVISIONS = 20;
};

class BakedCylinder final : public BakedColorDrawable {
public:
	explicit BakedCylinder(
		const glm::vec3& center = glm::vec3{ 0.0f, 0.0f, -2.0f },
		const float radius = 2.0f,
		const float height = 4.0f,
		const glm::vec3 up = glm::vec3{ 0.0f, 0.0f, 1.0f }
	) : _center{ center }, _radius{ radius }, _height{ height }, _up{ normalize(up) } {
		if (radius <= 0.0f) {
			throw std::exception{ "The radius of the cylinder is not positive\n" };
		}

		if (height <= 0.0f) {
			throw std::exception{ "The height of the cylinder is not positive\n" };
		}
	}

	[[nodiscard]] std::vector<float> vertices() const override;

	[[nodiscard]] std::vector<Primitive> primitives() const override;

private:
	const glm::vec3 _center;

	const float _radius;

	const float _height;

	const glm::vec3 _up;

	static constexpr auto SEGMENTS = 100;
};