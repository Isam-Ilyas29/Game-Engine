#include "keyboard.h"


//Process all input
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {

		//'Esc'
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			//PRESS = Close window
			glfwSetWindowShouldClose(window, true);
		}
		else if (action == GLFW_RELEASE) {
			//RELEASE = Pass
			break;
		}
		else if (action == GLFW_REPEAT) {
			//HELD_DOWN = Pass
			break;
		}
		break;
	}
}

void cameraInput(GLFWwindow* window) {
	extern glm::vec3 camera_pos, camera_front, camera_up;
	extern float delta_time;

	float camera_speed = 15.0f * delta_time;

	float cameraSpeed = 2.5 * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_pos += cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_pos -= cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	extern bool first_mouse;
	extern float yaw, pitch, lastX, lastY, fov;
	extern glm::vec3 camera_front;

	if (first_mouse)
	{
		lastX = xpos;
		lastY = ypos;
		first_mouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;	//Reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f; 
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	//Makes sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	extern float fov;

	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

//Allows user to alternate between polygon modes
void PolygonToggle::toggle(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		polygon_mode = !polygon_mode;
	}
}

