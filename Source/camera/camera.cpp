#include <iostream>

#include "camera.h"
#include "../Context/context.h"
#include "../Tools/tool.h"

#include <GLFW/glfw3.h>



/*----------------------------------------------------------------------------------*/

Camera::Camera(bool first_mouse, float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance)
	:mFirstMouse(first_mouse), mYaw(yaw), mPitch(pitch), mLastX(lastX), mLastY(lastY), mFOV(FOV), mRenderDistance(render_distance), mCameraSpeed(0.0f) {
	
	// Default settings
	mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mSpawnPostion = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::operator()(glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion) {
	mCameraPos = camera_pos;
	mCameraFront = camera_front;
	mCameraUp = camera_up;

	mSpawnPostion = spawn_postion;
}

/*----------------------------------------------------------------------------------*/

void Camera::perFrameTimeLogic() {
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;

	mCameraSpeed = 2.5 * delta_time;
}

/*----------------------------------------------------------------------------------*/

void Camera::moveForward() {
	mCameraPos += mCameraSpeed * mCameraFront;
}
void Camera::moveLeft() {
	mCameraPos -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
}
void Camera::moveBackward() {
	mCameraPos -= mCameraSpeed * mCameraFront;
}
void Camera::moveRight() {
	mCameraPos += glm::normalize(glm::cross(mCameraFront, mCameraUp)) * mCameraSpeed;
}

/*----------------------------------------------------------------------------------*/

void Camera::beginCursorRotation() {
	mFirstMouse = true;
}
void Camera::cursorRotation(double xPos, double yPos) {
	if (mFirstMouse) {
		mLastX = xPos;
		mLastY = yPos;
		mFirstMouse = false;
	}

	float xoffset = xPos - mLastX;
	float yoffset = mLastY - yPos;	// Reversed since y-coordinates go from bottom to top
	mLastX = xPos;
	mLastY = yPos;

	const float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mYaw += xoffset;
	mPitch += yoffset;

	// Makes sure that when pitch is out of bounds, screen doesn't get flipped
	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mCameraFront = glm::normalize(front);
}

/*----------------------------------------------------------------------------------*/

void Camera::zoom(double xoffset, double yoffset) {
	if (mFOV >= 1.0f && mFOV <= 45.0f)
		mFOV -= yoffset;
	if (mFOV <= 1.0f)
		mFOV = 1.0f;
	if (mFOV >= 45.0f)
		mFOV = 45.0f;
}

/*----------------------------------------------------------------------------------*/

glm::mat4 Camera::getMat4Projection() {
	glm::mat4 projection = glm::perspective(glm::radians(mFOV), context::window::aspectRatio(), 0.1f, mRenderDistance);

	return projection;
}

glm::mat4 Camera::getMat4View() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(mCameraPos, mCameraPos + mCameraFront, mCameraUp);

	return view;
}

glm::mat4 Camera::getMat4Transform() {
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, mSpawnPostion);
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}


