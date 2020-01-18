#pragma once

#include <glad/glad.h>

#include "../input/callback.h"
#include "../window/window.h"


void initialiseGLFW();
void setupWindow(GLFWwindow* window);
void setupContext(GLFWwindow* window);
void inputContext(GLFWwindow* iWindow, int key, int scancode, int action, int mods);
void initialiseGlad();
