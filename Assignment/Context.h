#pragma once

#include <glad/glad.h> // GLAD must be included before GLFW
#include <GLFW/glfw3.h>
#include <string_view>
#include <functional>
#include <map>
#include <memory>

class Context {
public:
	~Context();
	Context(const Context&) = delete;
	Context(Context&&) noexcept = delete;
	Context& operator=(const Context&) = delete;
	Context& operator=(Context&&) noexcept = delete;

	enum class Key {
		ESCAPE = GLFW_KEY_ESCAPE,
		F = GLFW_KEY_F,
		T = GLFW_KEY_T,
		W = GLFW_KEY_W
	};
	static std::unique_ptr<Context> create(
		std::string_view name = "Computer Graphics", 
		int width = 800, int height = 600
	);

	void setClose(bool close) const;

	void registerFramebufferCallback(const std::function<void(int, int)>& callback) const;

	[[nodiscard]] float getInitialRatio() const;

	void bindKey(Key key, const std::function<void()>& callback);

	void setMouseScrollCallback(const std::function<void(float)>& callback) const;

	void setMouseDragPerpetualCallback(const std::function<void(float, float)>& callback) const;

	void loop(const std::function<void()>& onFrame);

	[[nodiscard]] float getCurrentTime() const;

	[[nodiscard]] float getDeltaTime() const;

	static constexpr auto VERSION_MAJOR = 4;
	static constexpr auto VERSION_MINOR = 4;

private:
	explicit Context(std::string_view name, int width, int height);

	GLFWwindow* _window;

	const float _initialRatio;

	float _deltaTime{ 0.0f };
	float _lastTime{ 0.0f };
	float _currentTime{ 0.0f };

	std::map<Key, std::function<void()>> _keyBindings{};

	void processInputs() const;

	class Factory {
	public:
		std::unique_ptr<Context> operator()(std::string_view name, int width, int height) const;
	};
};
