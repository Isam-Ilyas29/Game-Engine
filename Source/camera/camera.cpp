#include "camera.h"


void framesPerSecond() {
	float frame_rate = 1.0 / delta_time;

	std::cout << "FPS: " << frame_rate << std::endl;
}

void Camera::perFrameTimeLogic() {
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
}

glm::mat4 Camera::getMat4Projection() {
	glm::mat4 projection = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	
	return projection;
}

glm::mat4 Camera::getMat4View() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

	return view;
}

glm::mat4 Camera::getMat4Transform() {
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	
	return transform;
}
