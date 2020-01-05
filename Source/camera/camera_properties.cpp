#include "camera_properties.h"

void cameraProperties::mouseProp (bool iFirstMouse, float iYaw, float iPitch, float iLastX, float iLastY, float iFOV) {
	first_mouse = iFirstMouse;
	yaw = iYaw;
	pitch = iPitch;
	lastX = iLastX;
	lastY = iLastY;
	FOV = iFOV;
}

//Mouse
extern bool first_mouse = first_mouse;
extern float yaw = yaw;
extern float pitch = pitch;
extern float lastX = lastX;
extern float lastY = lastY;
extern float FOV = FOV;

//Timing
extern float delta_time = 0.0f;
extern float last_frame = 0.0f;

//Camera
extern glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
extern glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
extern glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

