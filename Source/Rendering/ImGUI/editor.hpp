#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"

#include "Utils/miscellaneous.hpp"
#include "Rendering/graphic.hpp"
#include "Rendering/texture.hpp"
#include "Input/input_responder.hpp"


#ifdef DEBUG_MODE
	namespace collapsingHeader {

		void texture(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent1, bool editor);
		void colour(bool editor);
		void miscellaneous(bool editor);

		void controlsText(bool help);
		void aboutText(bool help);
	}

	bool isMouseOverUI();
#endif
