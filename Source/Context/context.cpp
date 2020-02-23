#include "context.h"

#include <glad/glad.h>

#include <imgui.h>
#include "../Rendering/ImGUI/imgui_impl_glfw.h"
#include "../Rendering/ImGUI/imgui_impl_opengl3.h"

#include "../Input/input_responder.h"


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


	void glfwContext() {
		glfwSwapInterval(1);

		glfwSetKeyCallback(::window, keyboardContext);
		glfwSetMouseButtonCallback(::window, mouseButtonContext);
		glfwSetCursorPosCallback(::window, cursorPosContext);
		glfwSetScrollCallback(::window, scrollContext);
	}

	void keyboardContext(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::onButton(key, action);
	}
	void mouseButtonContext(GLFWwindow* window, int button, int action, int mods) {
		input::onButton(button, action);
	}
	void cursorPosContext(GLFWwindow* window, double xpos, double ypos) {
		input::onCursorPos(xpos, ypos);
	}
	void scrollContext(GLFWwindow* window, double xoffset, double yoffset) {
		input::onScroll(xoffset, yoffset);
	}

	void imguiContext() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplGlfw_InitForOpenGL(::window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
		
		ImGui::StyleColorsDark();
	}
	void createImguiWindow(std::string name) {
		// Feed input to imGUI, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(144, 157.5), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(8, 8), ImGuiCond_Appearing);

		ImGui::Begin(name.data());
	}
	void renderImgui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	namespace window {

		bool setupWindow(std::string name, unsigned int width, unsigned int height) {
			::name = name;  ::scr_width = width; ::scr_height = height;
			::window = glfwCreateWindow(::scr_width, ::scr_height, ::name.data(), NULL, NULL);

			windowLoader(::window);
			bool success = windowVerifier(::window);

			glfwContext();
			return success;
		}
		void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			// Makes sure the viewport matches the new window dimensions
			glViewport(0, 0, width, height);

			ImGui::SetWindowSize("###GUI1", ImVec2(0.24 * width, 0.35 * height));
			ImGui::SetWindowPos("###GUI1", ImVec2(8, 8));
		}   // Whenever the window is resized (by OS or user resize) this callback function executes

		void closeWindow() {
			glfwSetWindowShouldClose(::window, true);
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

