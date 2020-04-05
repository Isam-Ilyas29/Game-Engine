#include "Rendering/editor.hpp"


namespace {

#ifdef IMGUI_LAYER
	bool is_editor_opened;
#endif
}


#ifdef IMGUI_LAYER
void setEditorWindow(bool predicate) {
	::is_editor_opened = predicate;
}
bool isEditorWindowOpened() {
	return ::is_editor_opened;
}
#endif
