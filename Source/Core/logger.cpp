#include "Core/logger.hpp"

#include "Core/utils.hpp"

#include <fmt/format.h>

#include <fstream>
#include <array>
#include <iostream>


/*----------------------------------------------------------------------------------*/

namespace {

	// File
	std::string log_path = "../../ExeDirectory/log.txt";
	std::ofstream log_file(log_path, std::ios::out);

	// Type specifier
	std::array<std::string, static_cast<u8>(logType::_COUNT)> log_type = { "INFO", "WARNING", "ERROR" };

#ifdef IMGUI_LAYER
	// GUI logs
	u64 gui_log_limit = 1000000000;

	std::deque<GUILogData> gui_log_data;
#endif

} // namespace

/*----------------------------------------------------------------------------------*/

// Logging
void log(logType type, std::string log_msg) {
	std::string log_type = ::log_type[static_cast<u8>(type)];
	std::string prefix = fmt::format("[{}] [{}]", getCurrentTime(), log_type);
	std::string full_log_msg = fmt::format("{} {}", prefix, log_msg);

	logToFile(full_log_msg);
	logToConsole(full_log_msg);
#ifdef IMGUI_LAYER
	logToGUI(prefix, log_msg, full_log_msg, type);
#endif
}

void logToFile(std::string log_msg) {
	::log_file << log_msg << "\n";
}
void logToConsole(std::string log_msg) {
	std::cerr << log_msg << std::endl;
}
#ifdef IMGUI_LAYER
	void logToGUI(std::string log_prefix, std::string log_msg, std::string full_log_msg, logType log_type) {
		if (::gui_log_data.size() >= ::gui_log_limit) {
			gui_log_data.pop_front();
		}
		GUILogData data{ log_prefix, log_msg, full_log_msg, log_type };
		gui_log_data.push_back(data);
	}
#endif

/*----------------------------------------------------------------------------------*/

#ifdef DEBUG_MODE
	// GUI Log
	std::deque<GUILogData> getGUILogData() {
		return ::gui_log_data;
	}

	void clearGUILog() {
		::gui_log_data.clear();
	}

	ImVec4 setGUILogTextColour(logType type) {
		switch (type) {
		case logType::INFO:
			return ImVec4(1.f, 1.f, 1.f, 1.f);
			break;
		case logType::WARNING:
			return ImVec4(0.9333f, 0.8235f, 0.0078f, 1.f);
			break;
		case logType::ERROR:
			return ImVec4(1.f, 0.f, 0.f, 1.f);
			break;
		}
		return ImVec4(0.f, 0.f, 0.f, 0.f);
	}
#endif

/*----------------------------------------------------------------------------------*/
