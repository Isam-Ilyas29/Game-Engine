#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
private:
	// Camera
	bool mClick;
	float mYaw;
	float mPitch;
	float mLastX;
	float mLastY;
	float mFOV;
	float mRenderDistance;

	float mCameraSpeed;

	glm::vec3 mCameraVelocity;

	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	glm::vec3 mSpawnPostion;

public:
	Camera(float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance);
	void operator() (glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion);

	float mSpeed;
	static const glm::vec3 mRight;
	static const glm::vec3 mUp;
	static const glm::vec3 mFront;

	void update(double delta_time);

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

