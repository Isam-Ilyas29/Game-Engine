#pragma once

#include "../Utils/std_types.hpp"

#include "../Input/input.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>


namespace context {
	void initialiseGLFW();
	bool initialiseGlad();

	void glfwContext();

	void keyboardContext(GLFWwindow* window, int key, int scancode, int action, int mods); 
	void mouseButtonContext(GLFWwindow* window, int button, int action, int mods);
	void cursorPosContext(GLFWwindow* window, double xpos, double ypos);
	void scrollContext(GLFWwindow* window, double xoffset, double yoffset);

	void imguiContext();
	void createImguiWindow(std::string name);
	void renderImgui();

	namespace window {
		bool setupWindow(std::string name, u16 width, u16 height);
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		void closeWindow();

		bool isClosed(GLFWwindow* window);

		void swapBuffers();
		void pollEvents();

		GLFWwindow* getWindow();
		unsigned int getWidth();
		unsigned int getHeight();

		float aspectRatio();
	}
}
