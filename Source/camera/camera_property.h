#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void cameraProperties (bool iFirstMouse, float iYaw, float iPitch, float iLastX, float iLastY, float iFOV, float iRenderDistance);


// Camera
extern bool first_mouse;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern float FOV;
extern float render_distance;

extern glm::vec3 camera_pos;
extern glm::vec3 camera_front;
extern glm::vec3 camera_up;

extern glm::vec3 spawn_postion;

extern glm::vec3 centre;

// Time
extern float delta_time;
extern float last_frame;


