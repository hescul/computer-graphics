#pragma once

#include <vector>
#include "Vertex.h"
#include "Mesh.h"

using IndexType = unsigned int;

struct Primitive {
	const int topology;
	const std::vector<IndexType> indices;
};

class Drawable {
public:
	virtual ~Drawable() = default;
	explicit Drawable(const GLuint program) : shader{ program } {}
	Drawable(const Drawable& other) = default;
	Drawable& operator=(const Drawable& other) = delete;
	Drawable(Drawable&& other) noexcept = default;
	Drawable& operator=(Drawable&& other) = delete;

	const GLuint shader;
	[[nodiscard]] virtual std::vector<float> vertices() const = 0;
	[[nodiscard]] virtual std::vector<GenericAttribute> layout() const = 0;
	[[nodiscard]] virtual std::vector<Primitive> primitives() const = 0;
};

class BakedColorDrawable : public Drawable {
public:
	BakedColorDrawable() : Drawable(loadBakedColorShader()) {}

	[[nodiscard]] std::vector<GenericAttribute> layout() const override;

private:
	static constexpr auto VERT_SHADER_PATH = "shaders/shader.vert";

	static constexpr auto FRAG_SHADER_PATH = "shaders/shader.frag";

	static [[nodiscard]] GLuint loadBakedColorShader();
};