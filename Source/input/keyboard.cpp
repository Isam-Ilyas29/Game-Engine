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

//Allows user to alternate between polygon modes
void PolygonToggle::toggle(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		polygon_mode = !polygon_mode;
	}
}

