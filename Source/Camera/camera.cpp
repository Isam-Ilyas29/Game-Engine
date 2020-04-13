#include "Camera/camera.hpp"

#include "Context/context.hpp"

#include <GLFW/glfw3.h>


/*----------------------------------------------------------------------------------*/

const glm::vec3 Camera::mRight = glm::vec3(1.f, 0.f, 0.f);
const glm::vec3 Camera::mUp = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 Camera::mFront = glm::vec3(0.f, 0.f, 1.f);

Camera::Camera(f32 yaw, f32 pitch, f32 lastX, f32 lastY, f32 FOV, f32 render_distance, f32 zoom_scale)
	: mClick(true), mYaw(yaw), mPitch(pitch), mLastX(lastX), mLastY(lastY), mFOV(FOV), mRenderDistance(render_distance), mCameraZoomScale(zoom_scale) {
	
	mCameraSpeed = 0.f;

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

void Camera::update(f32 delta_time) {
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
void Camera::cursorRotation(f32 xPos, f32 yPos) {
	if (mClick) {
		mLastX = xPos;
		mLastY = yPos;
		mClick = false;
	}

	f32 xoffset = xPos - mLastX;
	f32 yoffset = mLastY - yPos;	// Reversed since y-coordinates go from bottom to top
	mLastX = xPos;
	mLastY = yPos;

	const f32 sensitivity = 0.05f;
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

void Camera::zoom(f32 xoffset, f32 yoffset) {
	if (mFOV >= 1.0f && mFOV <= 45.0f)
		mFOV -= yoffset * mCameraZoomScale;
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
	transform = glm::rotate(transform, (f32)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	return transform;
}

/*----------------------------------------------------------------------------------*/
