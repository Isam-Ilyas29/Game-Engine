#pragma once

#include <glad/glad.h>

#include "../Input/input.h"


namespace context {
	void initialiseGLFW();
	bool initialiseGlad();

	void setupContext();
	void inputContext(GLFWwindow* window, int key, int scancode, int action, int mods);

	namespace window {
		bool setupWindow(std::string name, unsigned int width, unsigned int height);
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* getWindow();
		unsigned int getWidth();
		unsigned int getHeight();
	}
}


