#include "Core/Assert/assert.hpp"

#include "Core/logger.hpp"

#include <fmt/format.h>


namespace os {
	bool isDebuggerAttached() {
		bool ret = false;
#if defined(LEVK_OS_WINX)
		ret = IsDebuggerPresent() != 0;
#elif defined(LEVK_OS_LINUX)
		char buffer[4096];

		auto const status_fd = ::open("/proc/self/status", O_RDONLY);
		if (status_fd == -1) {
			return false;
		}

		auto const num_read = ::read(status_fd, buffer, sizeof(buffer) - 1);
		if (num_read <= 0) {
			return false;
		}

		buffer[num_read] = '\0';
		constexpr char tracerPidString[] = "TracerPid:";
		auto const tracer_pid_ptr = ::strstr(buffer, tracerPidString);
		if (!tracer_pid_ptr) {
			return false;
		}

		for (char const* characterPtr = tracer_pid_ptr + sizeof(tracerPidString) - 1; characterPtr <= buffer + num_read; ++characterPtr) {
			if (::isspace(*characterPtr)) {
				continue;
			}
			else {
				ret = ::isdigit(*characterPtr) != 0 && *characterPtr != '0';
			}
		}
#endif
		return ret;
	}

	void debugBreak() {
#if _MSC_VER
		__debugbreak();
#elif __linux__ || __MINGW32__
#ifdef SIGTRAP
		raise(SIGTRAP);
#endif
#endif
	}
}


void assertMessage(bool predicate, std::string_view message, std::filesystem::path file, u64 line_number) {
	if (predicate) {
		return;
	}

	log(logType::ERROR, fmt::format("ASSERTION FAILED | {} | \"{}\", \"Line. {}\"", message, file.generic_string(), line_number));
	
	if (os::isDebuggerAttached()) {
		os::debugBreak();
	}
	else {
		std::abort();
	}

	return;
}
