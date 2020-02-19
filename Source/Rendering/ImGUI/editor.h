#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "../../Utils/miscellaneous.h"
#include "../graphic.h"
#include "../texture.h"
#include "../../Input/input_responder.h"


#ifdef DEBUG_MODE
	namespace collapsingHeader {

		void texture(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent1, bool editor);
		void colour(bool editor);
		void miscellaneous(bool editor);

		void controlsText(bool help);
		void aboutText(bool help);
	}
#endif