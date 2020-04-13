#include "Core/std_types.hpp"
#include "Core/time.hpp"
#include "Context/context.hpp"
#include "Rendering/texture.hpp"

#include <imgui.h>

#include <vector>
#include <string>
#include <memory>


namespace imguiCategory {

	/*----------------------------------------------------------------------------------*/

	// Editor

#ifdef IMGUI_LAYER
	class EditorGUI {
	private:
		ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.25, context::window::getHeight() * 0.98f);
		ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.7465f, context::window::getHeight() * 0.0125);

		// Colour
		ImVec4 mFinalColour;

	public:
		// Texture
		static const char* mCurrentItem;
		static u8 mSelectedValue;
		static bool mApplyTexture;
		static bool mApplyTransparentOverlay;

		// Colour
		static ImVec4 mLocalColour;
		static bool mApplyBackground;

		// Polygon Mode
		static int mSelectedItem;
		static const char* mItems[];

		void process(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture);
		
		const ImVec4 getBackgroundColour();
	};
#endif

	/*----------------------------------------------------------------------------------*/

	// Scene

#ifdef IMGUI_LAYER
	class SceneGUI {
	private:
		ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.5f, context::window::getHeight() * 0.5f);
		ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.5f, context::window::getHeight() * 0.5f);

		u16 mPreviousWidth = 0;
		u16 mPreviousHeight = 0;

		bool isResizing = false;

		unsigned int mFBO;
		unsigned int mTexture;
		unsigned int mRBO;
	public:
		SceneGUI();

		void process();

		const unsigned int getFBO();
		const unsigned int getTexture();
		const unsigned int getRBO();
	};
#endif

	/*----------------------------------------------------------------------------------*/

	// Logger

#ifdef IMGUI_LAYER
	class LoggerGUI {
	private:
		ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.3476f);
		ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.005);

	public:
		static char mFind[250];
		static bool mInfoCategory;
		static bool mWarningCategory;
		static bool mErrorCategory;

		void process();
	};
#endif

	/*----------------------------------------------------------------------------------*/

	// Profiler

#ifdef IMGUI_LAYER
	class ProfilerGUI {
	private:
		ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.36f, context::window::getHeight() * 0.38f);
		ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.005f, context::window::getHeight() * 0.6135f);

		u16 mFPS;
	public:
		void process(Time delta_time);
	};
#endif

	/*----------------------------------------------------------------------------------*/

	// Console

#ifdef IMGUI_LAYER
	class ConsoleGUI {
	private:
		ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.35f);
		ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.6455f);

	public:
		void process();
	};
#endif

	/*----------------------------------------------------------------------------------*/

	// Text

#ifdef IMGUI_LAYER

#endif

	/*----------------------------------------------------------------------------------*/

} // namespace GUI

#ifdef IMGUI_LAYER
	bool shouldIsolate();
#endif
