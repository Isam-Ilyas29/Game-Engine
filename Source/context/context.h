#pragma once

#include <glad/glad.h>

#include "../input/input.h"


void windowLoader(GLFWwindow* window);
int windowVerifier(GLFWwindow* window);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void initialiseGLFW();
void setupWindow(GLFWwindow* window);
void setupContext(GLFWwindow* window);
void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods);
int initialiseGlad();
