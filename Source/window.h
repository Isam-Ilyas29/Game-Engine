#pragma once

#include <iostream>

#include <GLFW/glfw3.h>

//Screen size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void windowLoader(GLFWwindow* window);
int windowVerifier(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);