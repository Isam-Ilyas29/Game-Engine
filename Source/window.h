#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

#include "input/callbacks.h"

//Screen size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

GLFWwindow* windowCreation(std::string str_name);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);