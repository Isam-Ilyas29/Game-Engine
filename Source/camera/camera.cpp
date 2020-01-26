#include <iostream>

#include "camera.h"
#include "../Context/context.h"
#include "../Tools/tool.h"

#include <GLFW/glfw3.h>



float framesPerSecond() {
	float frame_rate = 1.0 / delta_time;
	return 0.f;
}

/*----------------------------------------------------------------------------------*/

Camera::Camera(bool first_mouse, float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance)
	:mFirstMouse(first_mouse), mYaw(yaw), mPitch(pitch), mLastX(lastX), mLastY(lastY), mFOV(FOV), mRenderDistance(render_distance), mDeltaTime(0.f), mLastFrame(0.f) {
	
	// Default settings
	mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mSpawnPostion = glm::vec3(0.0f, 0.0f, 0.0f);

	mCentre = mCameraPos + mCameraFront;
}

void Camera::operator()(glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion) {
	mCameraPos = camera_pos;
	mCameraFront = camera_front;
	mCameraUp = camera_up;

	mSpawnPostion = spawn_postion;

	mCentre = mCameraPos + mCameraFront;
}

/*----------------------------------------------------------------------------------*/

void Camera::perFrameTimeLogic() {
	float current_frame = glfwGetTime();
	mDeltaTime = current_frame - mLastFrame;
	mLastFrame = current_frame;
}

/*----------------------------------------------------------------------------------*/

glm::mat4 Camera::getMat4Projection() {
	glm::mat4 projection = glm::perspective(glm::radians(mFOV), context::window::aspectRatio(), 0.1f, mRenderDistance);

	return projection;
}

glm::mat4 Camera::getMat4View() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(mCameraPos, mCentre, mCameraUp);

	return view;
}

glm::mat4 Camera::getMat4Transform() {
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, mSpawnPostion);
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}


