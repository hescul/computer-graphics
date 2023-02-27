#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <functional>
#include <map>

class Context {
public:
	enum class Key {
		ESCAPE = GLFW_KEY_ESCAPE
	};

	explicit Context(std::string_view name);

	void setClose(bool close) const;

	void bindKey(Key key, const std::function<void()>& callback);

	void loop(const std::function<void()>& onRender) const;

	static void terminate();

	static constexpr auto WIDTH  = 800;
	static constexpr auto HEIGHT = 600;

	static constexpr auto VERSION_MAJOR = 3;
	static constexpr auto VERSION_MINOR = 3;

private:
	GLFWwindow* _window;

	std::map<Key, std::function<void()>> _keyBindings{};

	void processInputs() const;
};
