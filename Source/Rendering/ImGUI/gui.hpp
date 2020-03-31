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
#ifdef IMGUI_LAYER
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

#ifdef IMGUI_LAYER
		void display(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
#endif
		void process(const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
	};

	// Colour
	class BackgroundColourUI {
	public:
		static ImVec4 colour;
		static bool apply_background;

#ifdef IMGUI_LAYER
		void display();
#endif
		void process();
	};

	// Polygon Mode
	class PolygonModeUI {
	public:
		static int mSelectedItem;
		static const char* mItems[];

#ifdef IMGUI_LAYER
		void display();
#endif
		void process();
	};

#ifdef IMGUI_LAYER
	void fpsText(Time delta_time);
	void controlsText();
	void aboutText();
#endif
}

#ifdef IMGUI_LAYER
bool isMouseOverGUI();
#endif
