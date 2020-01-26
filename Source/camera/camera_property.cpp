#include "camera_property.h"

void cameraProperties(bool iFirstMouse, float iYaw, float iPitch, float iLastX, float iLastY, float iFOV, float iRenderDistance) {
	first_mouse = iFirstMouse;
	yaw = iYaw;
	pitch = iPitch;
	lastX = iLastX;
	lastY = iLastY;
	FOV = iFOV;
	render_distance = iRenderDistance;
}


// Camera
bool first_mouse;
float yaw;
float pitch;
float lastX;
float lastY;
float FOV;
float render_distance;

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 spawn_postion = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 centre = camera_pos + camera_front;

// Time
float delta_time = 0.0f;
float last_frame = 0.0f;


