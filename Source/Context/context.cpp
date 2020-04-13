#include "Context/context.hpp"

#include "Input/input_responder.hpp"
#include "Core/logger.hpp"
#include "Rendering/graphic.hpp"

#include <glad/glad.h>
#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"
#include "Rendering/ImGUI/icons_font_awesome.hpp"
#include <fmt/format.h>


/*----------------------------------------------------------------------------------*/

namespace {

	// Window
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

	// Viewport 
	unsigned int viewport_width;
	unsigned int viewport_height;

} // namespace

/*----------------------------------------------------------------------------------*/

void context::initialiseGLFW() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	log(logType::INFO, "CONTEXT | GLFW Successfully Initialised");
}

void context::glfwContext() {
	glfwSwapInterval(1);

	glfwSetKeyCallback(::window, keyboardContext);
	glfwSetMouseButtonCallback(::window, mouseButtonContext);
	glfwSetCursorPosCallback(::window, cursorPosContext);
	glfwSetScrollCallback(::window, scrollContext);
}

void context::keyboardContext(GLFWwindow* window, int key, int scancode, int action, int mods) {
	input::onButton(key, action);
}
void context::mouseButtonContext(GLFWwindow* window, int button, int action, int mods) {
	input::onButton(button, action);
}
void context::cursorPosContext(GLFWwindow* window, double xpos, double ypos) {
	input::onCursorPos(xpos, ypos);
}
void context::scrollContext(GLFWwindow* window, double xoffset, double yoffset) {
	input::onScroll(xoffset, yoffset);
}

/*----------------------------------------------------------------------------------*/

// ImGUI

#ifdef IMGUI_LAYER
	void context::imguiContext() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui_ImplGlfw_InitForOpenGL(::window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		log(logType::INFO, "CONTEXT | ImGui Successfully Initialised");
	}
	void context::setImguiTheme(std::function<void()> theme) {
		theme();
	}
	void context::setImguiTextFont(const std::filesystem::path& path, f32 size) {
		ImGui::GetIO().Fonts->AddFontFromFileTTF(path.generic_string().data(), size);
	}
	void context::setImguiIconFont(const std::filesystem::path& path, f32 size) {
		ImFontConfig icons_config; 
		icons_config.MergeMode = true; 
		icons_config.PixelSnapH = true;
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
		ImGui::GetIO().Fonts->AddFontFromFileTTF(path.generic_string().data(), size, &icons_config, icons_ranges);
	}
	void context::beginImguiFrame() {
		// Feed input to imGUI, start new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void context::renderImgui() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
#endif

/*----------------------------------------------------------------------------------*/

// Window

bool context::window::setupWindow(std::string_view name, u16 width, u16 height) {
	::name = name;  ::scr_width = width; ::scr_height = height;
	::window = glfwCreateWindow(::scr_width, ::scr_height, ::name.data(), NULL, NULL);

	windowLoader(::window);
	bool success = windowVerifier(::window);

	glfwContext();
	return success;
}


void context::window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	::scr_width = width; ::scr_height = height;

#ifdef IMGUI_LAYER
	// Re-size/Re-position GUI
	ImGui::SetWindowSize("###log1", ImVec2(width * 0.34f, height * 0.3476f));
	ImGui::SetWindowPos("###log1", ImVec2(width * 0.655f, width * 0.005));

	ImGui::SetWindowSize("###profiler1", ImVec2(width * 0.36f, height * 0.38f));
	ImGui::SetWindowPos("###profiler1", ImVec2(width * 0.005f, height * 0.6135f));

	ImGui::SetWindowSize("###console1", ImVec2(width * 0.34f, height * 0.35f));
	ImGui::SetWindowPos("###console1", ImVec2(width * 0.655f, height * 0.6455f));

	ImGui::SetWindowSize("###editor1", ImVec2(width * 0.25, height * 0.98f));
	ImGui::SetWindowPos("###editor1", ImVec2(width * 0.7465f, height * 0.0125));
#else
	GLAD_CHECK_ERROR(glViewport(0, 0, width, height));
#endif		
}


void context::window::closeWindow() {
	glfwSetWindowShouldClose(::window, true);
}
bool context::window::isClosed(GLFWwindow* window) {
	return glfwWindowShouldClose(window);
}

void context::window::swapBuffers() {
	glfwSwapBuffers(::window);
}
void context::window::pollEvents() {
	glfwPollEvents();
}


GLFWwindow* context::window::getWindow() {
	return ::window;
}
unsigned int context::window::getWidth() {
	return ::scr_width;
}
unsigned int context::window::getHeight() {
	return ::scr_height;
}


float context::window::aspectRatio() {
	if (::scr_width != 0 && ::scr_height != 0) {
		return (float)::scr_width / (float)::scr_height;
	}
	return 1.f;
}

/*----------------------------------------------------------------------------------*/