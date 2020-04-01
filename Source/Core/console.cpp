#include "Core/console.hpp"


namespace {

#ifdef IMGUI_LAYER
	bool is_console_opened;
#endif
}


#ifdef IMGUI_LAYER
void setConsoleWindow(bool predicate) {
	::is_console_opened = predicate;
}
bool isConsoleWindowOpened() {
	return ::is_console_opened;
}
#endif
