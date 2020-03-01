#pragma once

#include "../Utils/std_types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera {
private:
	// Camera
	bool mClick;
	f32 mYaw;
	f32 mPitch;
	f32 mLastX;
	f32 mLastY;
	f32 mFOV;
	f32 mRenderDistance;
	f32 mCameraZoomScale;

	f32 mCameraSpeed;

	glm::vec3 mCameraVelocity;

	glm::vec3 mCameraPos;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;

	glm::vec3 mSpawnPostion;

public:
	Camera(f32 yaw, f32 pitch, f32 lastX, f32 lastY, f32 FOV, f32 render_distance, f32 zoom_scale);
	void operator() (glm::vec3 camera_pos, glm::vec3 camera_front, glm::vec3 camera_up, glm::vec3 spawn_postion);

	f32 mSpeed;
	static const glm::vec3 mRight;
	static const glm::vec3 mUp;
	static const glm::vec3 mFront;

	void update(f64 delta_time);

	void moveForward();
	void moveLeft();
	void moveBackward();
	void moveRight();

	void beginCursorRotation();
	void cursorRotation(f64 xpos, f64 ypos);

	void zoom(f64 xoffset, f64 yoffset);

	glm::mat4 getMat4Projection();
	glm::mat4 getMat4View();
	glm::mat4 getMat4Transform();
};
