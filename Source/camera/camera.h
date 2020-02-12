#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
private:
	// Camera
	bool mFirstMouse;
	float mYaw;
	float mPitch;
	float mLastX;
	float mLastY;
	float mFOV;
	float mRenderDistance;

	float mCameraSpeed;

	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	glm::vec3 mSpawnPostion;

public:
	Camera(bool first_mouse, float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance);
	void operator() (glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion);

	void perFrameTimeLogic();

	void moveForward();
	void moveLeft();
	void moveBackward();
	void moveRight();

	void beginCursorRotation();
	void cursorRotation(double xpos, double ypos);

	void zoom(double xoffset, double yoffset);

	glm::mat4 getMat4Projection();
	glm::mat4 getMat4View();
	glm::mat4 getMat4Transform();
};

