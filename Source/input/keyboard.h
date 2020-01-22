#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera/camera_property.h"


void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);

void cameraInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class PolygonToggle {
public:
	bool polygon_mode;
	void toggle(GLFWwindow* window);
};