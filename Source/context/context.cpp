#include "context.h"


void initialiseGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool setupWindow(GLFWwindow* window) {
	windowLoader(window);
	bool success = windowVerifier(window);
	return success;
}

void setupContext(GLFWwindow* window) {
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, inputContext);
}

void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods) {
	input::onInput(key, action);
}

bool initialiseGlad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	return true;
}
