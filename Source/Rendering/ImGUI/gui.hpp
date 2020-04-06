#include "Core/std_types.hpp"
#include "Core/time.hpp"
#include "Rendering/texture.hpp"

#include <imgui.h>

#include <vector>
#include <string>
#include <memory>


namespace collapsingHeader {

	// Information
	class InformationUI {
	private:
		u16 mFPS;
	public:
#ifdef IMGUI_LAYER
		void header();
#endif
		void process(Time delta_time);
	};

	// Logger
	class LoggerUI {
	private:
		size_t mLowerIndex = 0;
		size_t mUpperIndex = 0;
	public:
#ifdef IMGUI_LAYER
		static char mFind[250];
		static bool mInfoCategory;
		static bool mWarningCategory;
		static bool mErrorCategory;

		void header();
		void process();
#endif
	};

	// Profiler
	class ProfilerUI {
	public:
#ifdef IMGUI_LAYER
		void header();
		void process();
#endif
	};

	// Console
	class ConsoleUI {
	public:
#ifdef IMGUI_LAYER
		void header();
		void process();
#endif
	};

	// Editor
	class EditorUI {
	public:
		// Texture
		static const char* mCurrentItem;
		static u8 mSelectedValue;
		static bool mApplyTexture;
		static bool mApplyTransparentOverlay;

		// Colour
		static ImVec4 mColour;
		static bool mApplyBackground;

		// Polygon Mode
		static int mSelectedItem;
		static const char* mItems[];

#ifdef IMGUI_LAYER
		void header();
#endif
		void process(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
	};

	// Text
#ifdef IMGUI_LAYER
	void controlsText();
	void aboutText();
#endif
}

#ifdef IMGUI_LAYER
bool isMouseOverGUI();
#endif
