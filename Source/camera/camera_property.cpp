#include "camera_property.h"

void mouseProp (bool iFirstMouse, float iYaw, float iPitch, float iLastX, float iLastY, float iFOV) {
	first_mouse = iFirstMouse;
	yaw = iYaw;
	pitch = iPitch;
	lastX = iLastX;
	lastY = iLastY;
	FOV = iFOV;
}

// Mouse
bool first_mouse;
float yaw;
float pitch;
float lastX;
float lastY;
float FOV;

// Timing
float delta_time = 0.0f;
float last_frame = 0.0f;

// Camera
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

