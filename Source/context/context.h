#pragma once

#include <glad/glad.h>

#include "../input/input.h"


void windowLoader(GLFWwindow* window);
bool windowVerifier(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void initialiseGLFW();
bool setupWindow(GLFWwindow* window);
void setupContext(GLFWwindow* window);
void inputContext(GLFWwindow* window, int key, int scancode, int action, int mods);
bool initialiseGlad();
