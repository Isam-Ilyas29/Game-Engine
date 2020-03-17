#include "Core/logger.hpp"

#include "Core/utils.hpp"

#include <fmt/format.h>

#include <fstream>
#include <array>


namespace {

	// File
	std::string log_path = "../ExeDirectory/log.txt";
	std::ofstream log_file(log_path, std::ios::out);
	bool is_log_opened = false;

	// Type specifier
	std::array<std::string, static_cast<u8>(logType::_COUNT)> log_type = { "INFO", "WARNING", "ERROR" };

#ifdef DEBUG_MODE
	// GUI logs
	std::vector<logType> log_colours;
	std::deque<std::string> log_messages;
#endif
}


// Logging
void log(logType type, std::string_view log_msg) {
	std::string lt = ::log_type[static_cast<u8>(type)];

	std::string full_log_msg = fmt::format("[{}] [{}] {}", getCurrentTime(), lt, log_msg);

	logToFile(full_log_msg);
	logToConsole(full_log_msg);
#ifdef DEBUG_MODE
	::log_colours.push_back(type);
	logToGUI(full_log_msg);
#endif
}

void logToFile(std::string log_msg) {
	::log_file << log_msg << "\n";
}
void logToConsole(std::string log_msg) {
	std::cerr << log_msg << std::endl;
}
#ifdef DEBUG_MODE
	void logToGUI(std::string log_msg) {
		if (::log_messages.size() >= 20) {
			log_messages.pop_front();
		}
		log_messages.push_back(log_msg);
	}
#endif


#ifdef DEBUG_MODE
	// GUI Log
	std::deque<std::string> getLog() {
		return ::log_messages;
	}
	std::vector<logType> getColour() {
		return ::log_colours;
	}

	ImVec4 setLogTextColour(logType type) {
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

	void setLogWindow(bool predicate) {
		::is_log_opened = predicate;
	}
	bool isLogWindowOpened() {
		return ::is_log_opened;
	}
#endif
