#pragma once

#include <GLFW/glfw3.h>


void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);

class PolygonToggle {
public:
	bool polygon_mode;
	void toggle(GLFWwindow* window);
};
