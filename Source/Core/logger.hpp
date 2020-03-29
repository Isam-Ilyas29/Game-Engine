#pragma once

#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"

#include <string>
#include <deque>
#include <vector>


// Determines severity of log
enum class logType {
	INFO,
	WARNING,
	ERROR,

	_COUNT
};

void log(logType type, std::string_view log_msg);

void logToFile(std::string log_msg);
void logToConsole(std::string log_msg);
#ifdef DEBUG_MODE
	void logToGUI(std::string log_msg);
#endif


#ifdef DEBUG_MODE
	extern logType log_text_colour;

	std::deque<std::string> getLog();
	std::vector<logType> getColour();

	ImVec4 setLogTextColour(logType type);

	void setLogWindow(bool predicate);
	bool isLogWindowOpened();
#endif
