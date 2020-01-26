#include <iostream>

#include "camera.h"
#include "camera_property.h"
#include "../Context/context.h"

#include <GLFW/glfw3.h>



float framesPerSecond() {
	float frame_rate = 1.0 / delta_time;
	return frame_rate;
}


namespace camera {

	void perFrameTimeLogic() {
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;
	}

	glm::mat4 getMat4Projection() {
		glm::mat4 projection = glm::perspective(glm::radians(FOV), context::window::aspectRatio(), 0.1f, render_distance);

		return projection;
	}

	glm::mat4 getMat4View() {
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(camera_pos, centre, camera_up);

		return view;
	}

	glm::mat4 getMat4Transform() {
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, spawn_postion);
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		return transform;
	}
}

