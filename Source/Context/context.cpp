#include "Context/context.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"
#include <fmt/format.h>

#include "Input/input_responder.hpp"
#include "Core/logger.hpp"


/*----------------------------------------------------------------------------------*/

namespace {
	GLFWwindow* window;
	std::string name = "None";
	u16 scr_width = 0;
	u16 scr_height = 0;

	void windowLoader(GLFWwindow* window) {
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, context::window::framebufferSizeCallback);
	}
	bool windowVerifier(GLFWwindow* window) {
		if (window == NULL) {
			log(logType::ERROR, "Failed to create GLFW window");
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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		log(logType::INFO, "CONTEXT | GLFW Successfully Initialised");
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


#ifdef IMGUI_LAYER
	void imguiContext() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui_ImplGlfw_InitForOpenGL(::window, true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		log(logType::INFO, "CONTEXT | ImGui Successfully Initialised");
	}
	void createImguiWindow(std::string name) {
		// Feed input to imGUI, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(0.24f * ::scr_width, 0.3476f * ::scr_height), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(8.f, 8.f), ImGuiCond_Appearing);

		ImGui::Begin(name.data());
	}
	void setImguiTheme(std::function<void()> theme) {
		theme();
	}
	void setImguiFont(const std::filesystem::path& font, f32 size) {
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF(font.generic_string().data(), size);
	}
	void renderImgui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
#endif


	namespace window {

		bool setupWindow(std::string_view name, u16 width, u16 height) {
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

			::scr_width = width; ::scr_height = height;

#ifdef IMGUI_LAYER
			ImGui::SetWindowSize("###GUI1", ImVec2(0.24f * width, 0.3476f * height));
			ImGui::SetWindowPos("###GUI1", ImVec2(8.f, 8.f));

			ImGui::SetWindowSize("###log1", ImVec2(0.34f * width, 0.3476f * height));
			ImGui::SetWindowPos("###log1", ImVec2(width - (width * 0.345f), 8.f));
#endif		
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
			if (::scr_width != 0 && ::scr_height != 0) {
				return (float)::scr_width / (float)::scr_height;
			}
			return 1.f;
		}
	}
}
