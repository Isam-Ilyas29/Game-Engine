#include "window.h"



void windowLoader(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

int windowVerifier(GLFWwindow* window) {
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
}

//Whenever the window is resized (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	//Makes sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}
