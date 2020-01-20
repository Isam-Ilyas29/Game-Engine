#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void mouseProp (bool iFirstMouse, float iYaw, float iPitch, float iLastX, float iLastY, float iFOV);


//Mouse
extern bool first_mouse;
extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern float FOV;

//Timing
extern float delta_time;
extern float last_frame;

//Camera
extern glm::vec3 camera_pos;
extern glm::vec3 camera_front;
extern glm::vec3 camera_up;


