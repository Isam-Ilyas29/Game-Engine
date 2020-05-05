#include "Core/std_types.hpp"
#include "Core/time.hpp"
#include "Context/context.hpp"
#include "Rendering/texture.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <vector>
#include <string>
#include <memory>

#ifdef IMGUI_LAYER
	namespace imguiCategory {

		/*----------------------------------------------------------------------------------*/

		// Dockspace and Menubar

		class DockspaceAndMenubarGUI {
		private:
			ImGuiWindowFlags mWindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;
			ImGuiDockNodeFlags mDockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

			ImGuiID mDockspaceID;

		public:
			void process();

			const ImGuiID getDockspaceID();
		};

		/*----------------------------------------------------------------------------------*/

		// Editor

		class EditorGUI {
		private:
			// Window properties
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

		/*----------------------------------------------------------------------------------*/

		// Scene

		class SceneGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(2000.f, 2000.f);
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

		/*----------------------------------------------------------------------------------*/

		// Console

		class ConsoleGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.35f);
			ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.6455f);

		public:
			void process();
		};

		/*----------------------------------------------------------------------------------*/

		// Logger

		class LoggerGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.3476f);
			ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.005);

		public:
			static char mFind[250];
			static bool mInfoCategory;
			static bool mWarningCategory;
			static bool mErrorCategory;

			void process();
		};

		/*----------------------------------------------------------------------------------*/

		// Profiler

		class ProfilerGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(context::window::getWidth() * 0.36f, context::window::getHeight() * 0.38f);
			ImVec2 mWindowPos = ImVec2(context::window::getWidth() * 0.005f, context::window::getHeight() * 0.6135f);

			u16 mFPS;
		public:
			void process(Time delta_time);
		};

		/*----------------------------------------------------------------------------------*/

	} // namespace GUI


	bool shouldIsolate();
#endif
