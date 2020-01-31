#pragma once

#include "../Input/input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>


namespace context {
	void initialiseGLFW();
	bool initialiseGlad();

	void glfwContext();
	void inputContext(GLFWwindow* window, int key, int scancode, int action, int mods);

	void imguiContext();

	namespace window {
		bool setupWindow(std::string name, unsigned int width, unsigned int height);
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		bool isClosed(GLFWwindow* window);
		void swapBuffers();
		void pollEvents();

		GLFWwindow* getWindow();
		unsigned int getWidth();
		unsigned int getHeight();

		float aspectRatio();
	}
}


