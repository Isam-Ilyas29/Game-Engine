#include "context.h"


/*----------------------------------------------------------------------------------*/

namespace {
	GLFWwindow* window;
	std::string name;
	unsigned int scr_width = 0;
	unsigned int scr_height = 0;

	void windowLoader(GLFWwindow* window) {
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, context::window::framebufferSizeCallback);
	}
	bool windowVerifier(GLFWwindow* window) {
		if (window == NULL) {
			std::cerr << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return false;
		}
		return true;
	}
}

/*----------------------------------------------------------------------------------*/

namespace context {

	void initialiseGLFW() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	bool initialiseGlad() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cerr << "Failed to initialize GLAD" << std::endl;
			return false;
		}
		glEnable(GL_DEPTH_TEST);
		return true;
	}

	void setupContext() {
		glfwSwapInterval(1);
		glfwSetKeyCallback(::window, inputContext);
	}

	void inputContext(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::onInput(key, action);
	}

	namespace window {

		bool setupWindow(std::string name, unsigned int width, unsigned int height) {
			::name = name;  ::scr_width = width; ::scr_height = height;
			::window = glfwCreateWindow(::scr_width, ::scr_height, ::name.data(), NULL, NULL);

			windowLoader(::window);
			bool success = windowVerifier(::window);

			setupContext();
			return success;
		}

		// Whenever the window is resized (by OS or user resize) this callback function executes
		void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			// Makes sure the viewport matches the new window dimensions
			glViewport(0, 0, width, height);
		}


		bool isClosed(GLFWwindow* window) {
			return glfwWindowShouldClose(window);
		}

		void swapBuffers() {
			glfwSwapBuffers(::window);
		}

		void pollEvents() {
			glfwPollEvents();
		}


		GLFWwindow* getWindow() {
			return ::window;
		}
		unsigned int getWidth() {
			return ::scr_width;
		}
		unsigned int getHeight() {
			return ::scr_height;
		}


		float aspectRatio() {
			return (float)::scr_width / (float)::scr_height;
		}
	}
}

