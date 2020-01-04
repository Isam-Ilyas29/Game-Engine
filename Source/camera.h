#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

extern bool first_mouse;
extern float yaw, pitch, lastX, lastY, fov, delta_time, last_frame;

void perFrameTimeLogic() {
	float currentFrame = glfwGetTime();
	delta_time = currentFrame - last_frame;
	last_frame = currentFrame;
}
