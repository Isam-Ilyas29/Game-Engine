#pragma once

#include <imgui.h>

#include <string>
#include <deque>


/*----------------------------------------------------------------------------------*/

// Determines severity of log
enum class LogType {
	INFO,
	WARNING,
	ERROR,

	_COUNT
};

void log(LogType type, std::string log_msg);

void logToFile(std::string log_msg);
void logToConsole(std::string log_msg);
#ifdef IMGUI_LAYER
	void logToGUI(std::string log_prefix, std::string log_msg, std::string full_log_msg, LogType log_type);
#endif

/*----------------------------------------------------------------------------------*/

#ifdef IMGUI_LAYER
	struct GUILogData {
		std::string prefix;
		std::string message;
		std::string full_log;
		LogType type;
	};

	std::deque<GUILogData> getGUILogData();

	void clearGUILog();

	ImVec4 setGUILogTextColour(LogType type);
#endif

/*----------------------------------------------------------------------------------*/
