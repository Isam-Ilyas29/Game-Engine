#include <iostream>

#include "camera.h"
#include "../Context/context.h"

#include <GLFW/glfw3.h>



/*----------------------------------------------------------------------------------*/

const glm::vec3 Camera::mRight = glm::vec3(1.f, 0.f, 0.f);
const glm::vec3 Camera::mUp = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 Camera::mFront = glm::vec3(0.f, 0.f, 1.f);

Camera::Camera(float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance)
	: mClick(true), mYaw(yaw), mPitch(pitch), mLastX(lastX), mLastY(lastY), mFOV(FOV), mRenderDistance(render_distance) {
	
	// Default settings
	mSpeed = 2.5f;

	mCameraVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	mCameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	mCameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mCameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mSpawnPostion = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::operator() (glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion) {
	mCameraPos = camera_pos;
	mCameraFront = camera_front;
	mCameraUp = camera_up;

	mSpawnPostion = spawn_postion;
}

/*----------------------------------------------------------------------------------*/

void Camera::update(double delta_time) {
	mCameraSpeed = mSpeed * delta_time;

	// Movement
	if (glm::length(mCameraVelocity) > 0.0f) {
		mCameraVelocity = glm::normalize(mCameraVelocity) * mCameraSpeed;
		mCameraPos += mCameraVelocity;
	}

	mCameraVelocity = glm::vec3(0.f, 0.f, 0.f);
}

/*----------------------------------------------------------------------------------*/

void Camera::moveForward() {
	glm::vec3 forward;
	forward = mCameraFront;
	mCameraVelocity += forward;
}
void Camera::moveLeft() {
	glm::vec3 left;
	left = glm::normalize(glm::cross(mCameraFront, mCameraUp));
	mCameraVelocity -= left;
}
void Camera::moveBackward() {
	glm::vec3 backward;
	backward = mCameraFront;
	mCameraVelocity -= backward;
}
void Camera::moveRight() {
	glm::vec3 right;
	right = glm::normalize(glm::cross(mCameraFront, mCameraUp));
	mCameraVelocity += right;
}

/*----------------------------------------------------------------------------------*/

void Camera::beginCursorRotation() {
	mClick = true;
}
void Camera::cursorRotation(double xPos, double yPos) {
	if (mClick) {
		mLastX = xPos;
		mLastY = yPos;
		mClick = false;
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


