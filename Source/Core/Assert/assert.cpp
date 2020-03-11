#include "Core/Assert/assert.hpp"


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
#if defined(LEVK_RUNTIME_MSVC)
		__debugbreak();
#elif defined(LEVK_RUNTIME_LIBSTDCXX)
#ifdef SIGTRAP
		raise(SIGTRAP);
#endif
#endif
	}
}


void assertMessage(bool predicate, std::string_view message, std::string_view file, u64 line_number) {
	if (predicate) {
		return;
	}

	std::cerr << '[' << std::filesystem::path(file).generic_string() << ']' << " [" << "Line. " << line_number << ']' << " [ASSERTION FAILED] " << message << std::endl;
	
	if (os::isDebuggerAttached()) {
#if defined(LEVK_OS_WINX)
		OutputDebugStringA(message.data());
#endif
		os::debugBreak();
	}
	else {
#if defined(LEVK_DEBUG)
		os::debugBreak();
#endif
	}
	return;
}
