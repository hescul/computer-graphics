#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "EntityManager.h"

class Camera : public EntityResource {
public:
	void setProjection(float left, float right, float bottom, float top, float zNear, float zFar);

	void setProjection(float fov, float ratio, float near, float far);

	[[nodiscard]] glm::mat4 getProjection() const;

	[[nodiscard]] glm::mat4 getViewMatrix() const;

	void relativeDrag(float offsetX, float offsetY);

	void relativeZoom(float amount);

	friend class Engine;

private:
	explicit Camera(const Entity entity, const float initialRatio) : EntityResource{ entity },
	_projection{ glm::perspective(glm::radians(DEFAULT_FOV), initialRatio, DEFAULT_NEAR, DEFAULT_FAR) }
	{}

	float _radius{ 4.0f };	// radius of the looking sphere [MIN_RADIUS; MAX_RADIUS]
	float _phi{ -90.0f };	// XY-plane rotation [0; 360]
	float _theta{ 80.0f };	// Z-axis rotation [1; 179]

	glm::mat4 _projection;

	static constexpr auto MIN_RADIUS = 1.0f;
	static constexpr auto MAX_RADIUS = 50.0f;

	static constexpr auto MIN_THETA = 1.0f;
	static constexpr auto MAX_THETA = 179.0f;

	static constexpr auto DEFAULT_FOV  = 45.0f;
	static constexpr auto DEFAULT_NEAR = 0.1f;
	static constexpr auto DEFAULT_FAR  = 100.0f;

	static constexpr auto ZOOM_SENSITIVE = 0.3f;
	static constexpr auto DRAG_SENSITIVE = 0.5f;
};