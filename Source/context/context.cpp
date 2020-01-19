#include "context.h"



void initialiseGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
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
    input::processInput(key, action);
}

int initialiseGlad(){
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	else {
		glEnable(GL_DEPTH_TEST);
	}
}
