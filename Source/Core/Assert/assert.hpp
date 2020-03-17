#include "Core/std_types.hpp"

#include <iostream>
#include <string>
#include <cassert>
#include <filesystem>

namespace os {
	bool isDebuggerAttached();
	void debugBreak();
} // namespace os


#define ASSERT(predicate, error_message) assertMessage(!!(predicate), error_message, __FILE__, __LINE__)

void assertMessage(bool predicate, std::string_view message, std::filesystem::path file, u64 line_number);
