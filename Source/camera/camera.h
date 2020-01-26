#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// TODO: Make tools module and TU.
float framesPerSecond();

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

	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	glm::vec3 mSpawnPostion;

	glm::vec3 mCentre;


	// Time
	float mDeltaTime;
	float mLastFrame;

public:
	Camera(bool first_mouse, float yaw, float pitch, float lastX, float lastY, float FOV, float render_distance);

	void operator() (glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion);

	void perFrameTimeLogic();

	glm::mat4 getMat4Projection();
	glm::mat4 getMat4View();
	glm::mat4 getMat4Transform();
};

