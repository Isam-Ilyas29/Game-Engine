#include "context.h"



void initialiseGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void setupWindow(GLFWwindow* window) {
	windowLoader(window);
	windowVerifier(window);
}

void setupContext(GLFWwindow* window) {
	glfwSwapInterval(1);

    glfwSetKeyCallback(window, inputContext);
}

void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods) {
    PlayerCallback::processInput(key, action);
}

void initialiseGlad(){
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
	}

	glEnable(GL_DEPTH_TEST);
}
