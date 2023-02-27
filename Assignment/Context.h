#pragma once

#include <glad/glad.h> // GLAD must be included before GLFW
#include <GLFW/glfw3.h>
#include <string_view>
#include <functional>
#include <map>

class Context {
public:
	enum class Key {
		ESCAPE = GLFW_KEY_ESCAPE,
		F = GLFW_KEY_F,
		T = GLFW_KEY_T,
		W = GLFW_KEY_W
	};

	enum class PolygonMode {
		LINE = GL_LINE,
		FILL = GL_FILL
	};

	explicit Context(std::string_view name = "Computer Graphics", int width = 800, int height = 600);

	void setClose(bool close) const;

	void togglePolygonMode();

	void setPolygonMode(PolygonMode mode);

	void bindKey(Key key, const std::function<void()>& callback);

	void loop(const std::function<void()>& onRender) const;

	static void terminate();

	static constexpr auto VERSION_MAJOR = 3;
	static constexpr auto VERSION_MINOR = 3;

private:
	GLFWwindow* _window;

	PolygonMode _currentMode{ PolygonMode::FILL };

	std::map<Key, std::function<void()>> _keyBindings{};

	void processInputs() const;
};
