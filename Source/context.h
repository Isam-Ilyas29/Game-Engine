#pragma once

#include <GLFW/glfw3.h>

#include "input/callback.h"


void setupContext(GLFWwindow* window);
void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods);