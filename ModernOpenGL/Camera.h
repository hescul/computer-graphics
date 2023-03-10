#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

class Camera {
public:
	enum class Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
	};

    glm::vec3 position;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    // When the field of view becomes smaller, the scene's projected space gets smaller.
	// This smaller space is projected over the same NDC, giving the illusion of zooming in.
    float zoom{ ZOOM };

    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

	explicit Camera(
        const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f),
        const glm::vec3 wUp  = glm::vec3(0.0f, 1.0f, 0.0f),
        const float yaw     = YAW,
        const float pitch   = PITCH
    )
	: position{ pos }, worldUp{ wUp }, yaw{ yaw }, pitch{ pitch },
        front{ updatedFront() }, right{ updatedRight() }, up{ updatedUp() } {}

	[[nodiscard]] glm::mat4 viewMatrix() const {
        return lookAt(position, position + front, up);
    }

    void processKeyboard(const Movement movement, const float deltaTime) {
        const auto velocity = SPEED * deltaTime;
        switch (movement) {
        case Movement::FORWARD:
            position += front * velocity;
            break;
        case Movement::BACKWARD: 
            position -= front * velocity;
            break;
        case Movement::LEFT: 
            position -= right * velocity;
            break;
        case Movement::RIGHT: 
            position += right * velocity;
            break;
        }
    }

    void processMouse(const float offsetX, const float offsetY, const bool pitchGuard = true) {
        yaw += offsetX * SENSE;
        pitch += offsetY * SENSE;

        // The pitch needs to be constrained in such a way that users won't be able to look higher
		// than 89 degrees (at 90 degrees we get the LookAt flip) and also not below -89 degrees.
        if (pitchGuard) {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void processScroll(const float offsetY) {
        zoom -= offsetY;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    void updateCameraVectors() {
        front = updatedFront();
        // We also re-calculate the right and up vector.
        // We should normalize the vectors, because their length gets closer to 0 the more we look up
        // or down which results in slower movement.
        right = updatedRight();
        up = updatedUp();
    }

	[[nodiscard]] glm::vec3 updatedFront() const {
        const auto updatedFront = glm::vec3{
            std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw)),
            std::sin(glm::radians(pitch)),
            std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw))
        };
        return normalize(updatedFront);
    }

    [[nodiscard]] glm::vec3 updatedRight() const {
	    return normalize(cross(front, worldUp));
    }

    [[nodiscard]] glm::vec3 updatedUp() const {
        return normalize(cross(right, front));
    }

    static constexpr auto YAW   = -90.0f;
    static constexpr auto PITCH = 0.0f;
    static constexpr auto SPEED = 2.5f;
    static constexpr auto SENSE = 0.1f;
    static constexpr auto ZOOM  = 45.0f;
};