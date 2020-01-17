#include "window.h"


GLFWwindow* windowCreation(std::string str_name) {
	const char* name = str_name.c_str();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name, NULL, NULL);

    //Window verifier
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }


	glfwSwapInterval(1);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, PlayerCallback::processInput);

	return window;
}

//[GLFW] Whenever the window is resized (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	//Makes sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}