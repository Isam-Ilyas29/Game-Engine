#include "Core/profiler.hpp"


namespace {

#ifdef IMGUI_LAYER
	bool is_profiler_opened;
#endif
}


#ifdef IMGUI_LAYER
	void setProfileWindow(bool predicate) {
		::is_profiler_opened = predicate;
	}
	bool isProfileWindowOpened() {
		return ::is_profiler_opened;
	}
#endif
