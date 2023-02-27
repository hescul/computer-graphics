#include "context.h"
#include <glad/glad.h>
#include <iostream>
#include <algorithm>

Context::Context(const std::string_view name) {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	// ensure when a user does not have the proper OpenGL version GLFW fails to run
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
	// access to smaller subset of OpenGL without backward-compatible features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// required by most of GLFW's functions
	_window = glfwCreateWindow(WIDTH, HEIGHT, name.data(), nullptr, nullptr);
	if (_window == nullptr) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		throw std::exception("Failed to create GLFW window\n");
	}
	// make the window the main context on the current thread
	glfwMakeContextCurrent(_window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to initialize GLAD\n";
		throw std::exception("Failed to initialize GLAD\n");
	}

	// viewport should be adjusted when the window is resized
	// callbacks should be registered before the rendering loop
	glfwSetFramebufferSizeCallback(
		_window, [](GLFWwindow* _, const int width, const int height) {
			// make sure the viewport matches the new window dimensions
			// width and height will be significantly larger than specified on retina displays
			glViewport(0, 0, width, height);
		}
	);
}

void Context::setClose(const bool close) const {
	glfwSetWindowShouldClose(_window, close);
}

void Context::terminate() {
	glfwTerminate();
}

void Context::bindKey(const Key key, const std::function<void()>& callback) {
	_keyBindings[key] = callback;
}

void Context::loop(const std::function<void()>& onRender) const {
	while (!glfwWindowShouldClose(_window)) {
		// check key presses and react accordingly every frame
		processInputs();

		// render
		onRender();

		// polling and swapping
		glfwPollEvents();
		glfwSwapBuffers(_window);
	}
}

void Context::processInputs() const {
	std::ranges::for_each(_keyBindings.begin(), _keyBindings.end(),
		[&](const std::pair<Key, std::function<void()>>& element) {
			if (glfwGetKey(_window, static_cast<int>(element.first)) == GLFW_PRESS) {
				element.second();
			}
		}
	);
}