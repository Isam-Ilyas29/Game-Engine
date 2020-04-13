#pragma once

#include "Core/std_types.hpp"
#include "Input/input.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <functional>
#include <filesystem>


namespace context {

	/*----------------------------------------------------------------------------------*/

	void initialiseGLFW();
	void glfwContext();

	void keyboardContext(GLFWwindow* window, int key, int scancode, int action, int mods); 
	void mouseButtonContext(GLFWwindow* window, int button, int action, int mods);
	void cursorPosContext(GLFWwindow* window, double xpos, double ypos);
	void scrollContext(GLFWwindow* window, double xoffset, double yoffset);

	/*----------------------------------------------------------------------------------*/

	// ImGUI

#ifdef IMGUI_LAYER
	void imguiContext();
	void setImguiTheme(std::function<void()> theme);
	void setImguiTextFont(const std::filesystem::path& path, f32 size);
	void setImguiIconFont(const std::filesystem::path& path, f32 size);
	void beginImguiFrame();
	void renderImgui();
#endif

	/*----------------------------------------------------------------------------------*/

	// Window

	namespace window {
		bool setupWindow(std::string_view name, u16 width, u16 height);
		void framebufferSizeCallback(GLFWwindow* window, int width, int height);

		void closeWindow();

		bool isClosed(GLFWwindow* window);

		void swapBuffers();
		void pollEvents();

		GLFWwindow* getWindow();
		unsigned int getWidth();
		unsigned int getHeight();

		float aspectRatio();

	} //namespace window

	/*----------------------------------------------------------------------------------*/

} // namespace context
