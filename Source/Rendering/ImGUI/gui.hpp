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

			bool mShouldDefaultDock = false;
			u8 mFrameCount = 0;

		public:
			void update();

			void defaultDock();

			const bool getShouldDefaultDock();

			const ImGuiID getDockspaceID();
		};

		/*----------------------------------------------------------------------------------*/

		// Editor

		class EditorGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(1920.f, 1006.f);
			ImVec2 mWindowPos = ImVec2(0.f, 25.f);

		public:

			void update();
		};

		/*----------------------------------------------------------------------------------*/

		// Scene

		class SceneGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(1920.f, 1006.f);
			ImVec2 mWindowPos = ImVec2(0.f, 25.f);

			u16 mPreviousWidth = 0;
			u16 mPreviousHeight = 0;

			bool isResizing = false;

			unsigned int mFBO;
			unsigned int mTexture;
			unsigned int mRBO;

		public:
			SceneGUI();

			void update(unsigned int post_processing_fbo_texture, unsigned int post_processing_rbo);

			const unsigned int getFBO();
			const unsigned int getTexture();
			const unsigned int getRBO();
		};

		/*----------------------------------------------------------------------------------*/

		// Console

		class ConsoleGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(1920.f, 1006.f);
			ImVec2 mWindowPos = ImVec2(0.f, 25.f);

		public:
			void update();
		};

		/*----------------------------------------------------------------------------------*/

		// Logger

		class LoggerGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(1920.f, 1006.f);
			ImVec2 mWindowPos = ImVec2(0.f, 25.f);

		public:
			static char mFind[250];
			static bool mInfoCategory;
			static bool mWarningCategory;
			static bool mErrorCategory;

			void update();
		};

		/*----------------------------------------------------------------------------------*/

		// Profiler

		class ProfilerGUI {
		private:
			// Window properties
			ImVec2 mWindowSize = ImVec2(1920.f, 1006.f);
			ImVec2 mWindowPos = ImVec2(0.f, 25.f);

			u16 mFPS;
		public:
			void update(Time delta_time);
		};

		/*----------------------------------------------------------------------------------*/

	} // namespace GUI


	bool shouldIsolate();
#endif
