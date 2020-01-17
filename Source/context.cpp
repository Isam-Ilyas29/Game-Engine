#include "context.h"



void setupContext(GLFWwindow* window) {
    glfwSetKeyCallback(window, inputContext);
}

void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods) {
    PlayerCallback::processInput(key, action);
}
