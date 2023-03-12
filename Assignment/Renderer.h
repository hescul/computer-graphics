#pragma once

#include <array>

#include "View.h"

using ClearColor = std::array<float, 4>;

class Renderer {
public:
	struct ClearOptions {
		bool clear{ true };

		std::array<float, 4> clearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
	};

	void setClearOptions(const ClearOptions& options);

	[[nodiscard]] ClearOptions getClearOptions() const;

	void render(View* view) const;

private:
	ClearOptions _clearOptions{};
};