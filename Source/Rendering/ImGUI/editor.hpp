#include "Core/std_types.hpp"

#include <imgui.h>
#include "Rendering/ImGUI/imgui_impl_glfw.hpp"
#include "Rendering/ImGUI/imgui_impl_opengl3.hpp"

#include <memory>
#include <array>

#include "Core/utils.hpp"
#include "Rendering/graphic.hpp"
#include "Rendering/texture.hpp"
#include "Input/input_responder.hpp"


namespace collapsingHeader {

	// Logger
	class LoggerUI {
	public:
#ifdef DEBUG_MODE
		void display();
#endif
		void process();
	};

	// Texture
	class TextureUI {
	public:
		static const char* mCurrentItem;
		static u8 mSelectedValue;
		static bool mApplyTexture;
		static bool mApplyTransparentOverlay;

#ifdef DEBUG_MODE
		void display(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
#endif
		void process(const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
	};

	// Colour
	class ColourUI {
	public:
		static ImVec4 colour;
		static bool apply_background;

#ifdef DEBUG_MODE
		void display();
#endif
		void process();
	};

	// Miscellaneous
	class MiscellaneousUI {
	public:
		static int mSelectedItem;
		static const char* mItems[];

#ifdef DEBUG_MODE
		void display();
#endif
		void process();
	};

#ifdef DEBUG_MODE
	void fpsText(Time delta_time);
	void controlsText();
	void aboutText();
#endif
}

#ifdef DEBUG_MODE
bool isMouseOverGUI();
#endif
