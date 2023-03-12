#pragma once

#include "Scene.h"

class View {
public:
	void setScene(Scene* scene);

	[[nodiscard]] Scene* getScene() const;

private:
	Scene* _scene{ nullptr };
};