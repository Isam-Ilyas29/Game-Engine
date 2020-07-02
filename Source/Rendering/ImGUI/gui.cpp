#include "Rendering/ImGUI/gui.hpp"

#include "Rendering/ImGUI/icons_font_awesome.hpp"
#include "Core/utils.hpp"
#include "Core/logger.hpp"
#include "Core/profiler.hpp"
#include "Core/console.hpp"
#include "Rendering/editor.hpp"
#include "Rendering/graphic.hpp"
#include "Core/rng.hpp"

#include "Rendering/ImGUI/imgui_external.hpp"
#include <fmt/format.h>

#include <deque>
#include <algorithm>


/*----------------------------------------------------------------------------------*/

#ifdef IMGUI_LAYER
	namespace {

		// Which windows to view
		bool should_scene_gui = true;
		bool should_editor_gui = true;
		bool should_logger_gui = true;
		bool should_profiler_gui = true;
		bool should_console_gui = true;

		// Isolates when not on scene gui
		bool should_isolate = false;

	} // namespace

	/*----------------------------------------------------------------------------------*/

	// Dockspace and Menubar

	void imguiCategory::DockspaceAndMenubarGUI::update() {

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

		ImGui::Begin("Dockspace", nullptr, mWindowFlags);
		{
			ImGui::PopStyleVar(3);

			mDockspaceID = ImGui::GetID("Dockspace");
			ImGui::DockSpace(mDockspaceID, ImVec2(0.f, 0.f), mDockspaceFlags);

			// Menu bar
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("View")) {
					// Scene
					if (::should_scene_gui) {
						if (ImGui::MenuItem("Scene  \t\t\t\t\t\t\t\t" ICON_FA_CHECK)) {
							::should_scene_gui = false;
						}
					}
					else {
						if (ImGui::MenuItem("Scene")) {
							::should_scene_gui = true;
						}
					}
					// Editor
					if (::should_editor_gui) {
						if (ImGui::MenuItem("Editor  \t\t\t\t\t\t\t\t" ICON_FA_CHECK)) {
							::should_editor_gui = false;
						}
					}
					else {
						if (ImGui::MenuItem("Editor")) {
							::should_editor_gui = true;
						}
					}
					// Logger
					if (::should_logger_gui) {
						if (ImGui::MenuItem("Logger \t\t\t\t\t\t\t\t" ICON_FA_CHECK)) {
							::should_logger_gui = false;
						}
					}
					else {
						if (ImGui::MenuItem("Logger")) {
							::should_logger_gui = true;
						}
					}
					// Profiler
					if (::should_profiler_gui) {
						if (ImGui::MenuItem("Profiler\t\t\t\t\t\t\t\t" ICON_FA_CHECK)) {
							::should_profiler_gui = false;
						}
					}
					else {
						if (ImGui::MenuItem("Profiler")) {
							::should_profiler_gui = true;
						}
					}
					// Console
					if (::should_console_gui) {
						if (ImGui::MenuItem("Console   \t\t\t\t\t\t\t" ICON_FA_CHECK)) {
							::should_console_gui = false;
						}
					}
					else {
						if (ImGui::MenuItem("Console")) {
							::should_console_gui = true;
						}
					}

					// Default docking
					if (ImGui::MenuItem("Default Layout")) {
						mShouldDefaultDock = true;
					}

					ImGui::EndMenu();
				}
				if (mShouldDefaultDock) {
					mFrameCount++;
				}
				if (mFrameCount == 2) {
					mShouldDefaultDock = false;
					mFrameCount = 0;
				}
				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
	}

	void imguiCategory::DockspaceAndMenubarGUI::defaultDock() {
		ImGui::DockBuilderRemoveNode(mDockspaceID);
		ImGui::DockBuilderAddNode(mDockspaceID, ImGuiDockNodeFlags_None);
		ImGui::DockBuilderSetNodeSize(mDockspaceID, ImGui::GetMainViewport()->Size);

		ImGuiID dock_main = mDockspaceID;
		ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Right, 0.24f, nullptr, &dock_main);
		ImGuiID dock_down = ImGui::DockBuilderSplitNode(dock_main, ImGuiDir_Down, 0.23f, nullptr, &dock_main);

		ImGui::DockBuilderDockWindow("Profiler##profiler1", dock_main);
		ImGui::DockBuilderDockWindow("Scene##scene1", dock_main);
		ImGui::DockBuilderDockWindow("Editor##editor1", dock_right);
		ImGui::DockBuilderDockWindow("Console##console1", dock_down);
		ImGui::DockBuilderDockWindow("Log##logger1", dock_down);
		ImGui::DockBuilderFinish(mDockspaceID);
	}

	const bool imguiCategory::DockspaceAndMenubarGUI::getShouldDefaultDock() {
		return mShouldDefaultDock;
	}

	const ImGuiID imguiCategory::DockspaceAndMenubarGUI::getDockspaceID() {
		return mDockspaceID;
	}

	/*----------------------------------------------------------------------------------*/

	// Editor

	// Texture
	const char* imguiCategory::EditorGUI::mCurrentItem = "None";
	u8 imguiCategory::EditorGUI::mSelectedValue = 0;
	bool imguiCategory::EditorGUI::mApplyTexture;
	bool imguiCategory::EditorGUI::mApplyTransparentOverlay;

	// Post-processing
	bool imguiCategory::EditorGUI::mDefault = true;
	bool imguiCategory::EditorGUI::mInvertedColour;
	bool imguiCategory::EditorGUI::mGreyscale;
	bool imguiCategory::EditorGUI::mSharpenedColours;
	bool imguiCategory::EditorGUI::mBlur;

	// Background Colour
	ImVec4 imguiCategory::EditorGUI::mLocalColour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
	bool imguiCategory::EditorGUI::mApplyBackground;

	// Polygon Mode
	int imguiCategory::EditorGUI::mSelectedItem;
	const char* imguiCategory::EditorGUI::mItems[] = { "Fill", "Line", "Point" };

	void imguiCategory::EditorGUI::update(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture) {
		
		if (::should_editor_gui) {
			// Set window size and pos
			ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_FirstUseEver);

			ImGui::Begin("Editor##editor1");
			{
				// Texture
				if (ImGui::CollapsingHeader("Textures")) {

					// Copes items of passed in vector into another vector of 'const char*'
					std::vector<const char*> items;
					items.push_back("None");
					for (size_t i = 0; i < textures.size(); i++) {
						items.push_back(textures[i].data());
					}

					// Texture loader combo:
					ImGui::TextWrapped("\nTexture Picker: ");
					if (ImGui::BeginCombo("##texture_picker1", mCurrentItem, ImGuiComboFlags_NoArrowButton)) {
						for (size_t i = 0; i < items.size(); i++) {
							bool is_selected = (mCurrentItem == items[i]);
							if (ImGui::Selectable(items[i], is_selected)) {
								mSelectedValue = i;		//Index of selected item
								mCurrentItem = items[i];	//Name of selected item
							}
							if (is_selected) {
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					ImGuiStyle& style = ImGui::GetStyle();
					float spacing = style.ItemInnerSpacing.x;


					// Creates arrows and makes sure they cannot go beyond/below possible value limit
					ImGui::SameLine(0, spacing);
					if (ImGui::ArrowButton("##left1", ImGuiDir_Left)) {
						if (mSelectedValue != 0) {
							mSelectedValue -= 1;
							mCurrentItem = items[mSelectedValue];
						}
					}
					ImGui::SameLine(0, spacing);
					if (ImGui::ArrowButton("##right1", ImGuiDir_Right)) {
						if (mSelectedValue != (items.size() - 1)) {
							mSelectedValue += 1;
							mCurrentItem = items[mSelectedValue];
						}
					}

					for (size_t i = 0; i < loaded_textures.size(); i++) {
						bool correct_texture = (i == mSelectedValue);

						// Makes sure if selected item is none no image preview will be loaded and in else statement it default loads the error texture
						if (correct_texture && mSelectedValue != 0) {
							// Image previewer
							GLuint imgui_preview_image_texture = 0;

							bool image = loaded_textures[i]->previewImage(&imgui_preview_image_texture);
							IM_ASSERT(image);

							ImGui::TextWrapped("\nImage Preview: ");
							ImGui::Image((void*)(intptr_t)imgui_preview_image_texture, ImVec2(160, 160));

							ImGui::TextWrapped("\n");
							ImGui::TextWrapped("Apply: ");
							ImGui::SameLine();
							ImGui::Checkbox("##apply_texture1", &mApplyTexture);

							break;
						}
					}

					// Transparent Overlay
					ImGui::TextWrapped("\n");
					ImGui::TextWrapped("Transparent Overlay: ");
					ImGui::SameLine();
					ImGui::Checkbox("##transparent_overlay1", &mApplyTransparentOverlay);

					ImGui::Text("\n");
				}

				// Post processing
				if (ImGui::CollapsingHeader("Post-processing")) {
					ImGui::Text("\n");

					if (ImGui::Checkbox("Default##post_processing_default1", &mDefault)) {
						mInvertedColour = false;
						mGreyscale = false;
						mSharpenedColours = false;
						mBlur = false;
						mPostProcessingType = 0;
					}
					if (ImGui::Checkbox("Inverted Colours##post_processing_inverted_colours1", &mInvertedColour)) {
						mDefault = false;
						mGreyscale = false;
						mSharpenedColours = false;
						mBlur = false;
						mPostProcessingType = 1;
					}
					if (ImGui::Checkbox("Greyscale##post_processing_grey_scale1", &mGreyscale)) {
						mDefault = false;
						mInvertedColour = false;
						mSharpenedColours = false;
						mBlur = false;
						mPostProcessingType = 2;
					}
					if (ImGui::Checkbox("Sharpened Colours##post_processing_sharpened_colours_1", &mSharpenedColours)) {
						mDefault = false;
						mInvertedColour = false;
						mGreyscale = false;
						mBlur = false;
						mPostProcessingType = 3;
					}
					if (ImGui::Checkbox("Blur##post_processing_blur1", &mBlur)) {
						mDefault = false;
						mInvertedColour = false;
						mGreyscale = false;
						mSharpenedColours = false;
						mPostProcessingType = 4;
					}

					ImGui::Text("\n");
				}

				// Background Colour
				if (ImGui::CollapsingHeader("Background Colour")) {
					ImGui::TextWrapped("\nBackground Colour Picker: ");
					ImGui::ColorEdit3("##background_colour_picker1", (float*)&mLocalColour);

					ImGui::TextWrapped("\n");
					ImGui::TextWrapped("Apply: ");
					ImGui::SameLine();
					ImGui::Checkbox("##apply_background1", &mApplyBackground);

					ImGui::Text("\n");
				}

				// Polygon Mode
				if (ImGui::CollapsingHeader("Polygon Mode")) {

					// Polygon toggle checkbox: 
					ImGui::TextWrapped("\nPolygon Mode: ");
					ImGui::ListBox("##polygon_mode1", &mSelectedItem, mItems, IM_ARRAYSIZE(mItems), 3);
					ImGui::Text("\n");
				}

			}
			ImGui::End();
		}

		// Texture
		for (size_t i = 0; i < loaded_textures.size(); i++) {
			bool correct_texture = (i == mSelectedValue);

			if (correct_texture && mSelectedValue != 0) {
				if (mApplyTexture) {
					loaded_textures[i]->bind(0);
				}
				else {
					error_texture.bind(0);
				}

				break;
			}
			else {
				error_texture.bind(0);
			}
		}

		if (mApplyTransparentOverlay) {
			transparent_texture.bind(1);
		}

		// Background Colour
		if (mApplyBackground) {
			mFinalColour = ImVec4(mLocalColour.x, mLocalColour.y, mLocalColour.z, mLocalColour.w);
		}
		else {
			mFinalColour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);	// Default
		}

		// Polygon Mode
		polygon_mode = static_cast<polygonMode>(mSelectedItem);
	}

	const ImVec4 imguiCategory::EditorGUI::getBackgroundColour() {
		return mFinalColour;
	}

	const u32 imguiCategory::EditorGUI::getPostProcessingType() {
		return mPostProcessingType;
	}

	/*----------------------------------------------------------------------------------*/

	// Scene

imguiCategory::SceneGUI::SceneGUI() {

	glViewport(0, 0, mWindowSize.x, mWindowSize.y);

	// FBO
	GLAD_CHECK_ERROR(glGenFramebuffers(1, &mFBO));
	GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

	// Creates a colour attachment texture
	GLAD_CHECK_ERROR(glGenTextures(1, &mTexture));
	GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, mTexture));
	GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWindowSize.x, mWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLAD_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0));

	// Creates a renderbuffer object for depth and stencil attachment
	GLAD_CHECK_ERROR(glGenRenderbuffers(1, &mRBO));
	GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
	GLAD_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWindowSize.x, mWindowSize.y));
	GLAD_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO));

	// Checks if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		log(logType::ERROR, "Framebuffer is not complete");
	}


	GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
void imguiCategory::SceneGUI::update(unsigned int post_processing_fbo_texture, unsigned int post_processing_rbo) {

	if (::should_scene_gui) {
		//Scene
		ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_FirstUseEver);
		ImGui::Begin("Scene##scene1", nullptr);
		{
			if (ImGui::IsWindowHovered()) {
				::should_isolate = false;
			}
			else {
				::should_isolate = true;
			}

			if (ImGui::GetWindowContentRegionWidth() != mPreviousWidth || ImGui::GetWindowContentRegionHeight() != mPreviousHeight) {
				isResizing = true;
			}
			if (isResizing && (ImGui::GetWindowContentRegionWidth() == mPreviousWidth || ImGui::GetWindowContentRegionHeight() == mPreviousHeight) && (ImGui::GetWindowContentRegionWidth() > 0 && ImGui::GetWindowContentRegionHeight() > 0)) {
				// Viewport
				GLAD_CHECK_ERROR(glViewport(0, 0, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight()));

				// Resizes the texture
				GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, post_processing_fbo_texture));
				GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
				GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, mTexture));
				GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));

				// Resizes the RBO
				GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
				GLAD_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight()));
				GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, post_processing_rbo));
				GLAD_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight()));
			}

			// Draw scene texture
			ImGui::Image((void*)mTexture, ImVec2(ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight()), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

			// Reset resizing
			isResizing = false;

			// Set previous width and height
			mPreviousWidth = ImGui::GetWindowContentRegionWidth();
			mPreviousHeight = ImGui::GetWindowContentRegionHeight();
		}
		ImGui::End();
	}
}

const unsigned int imguiCategory::SceneGUI::getFBO() {
	return mFBO;
}
const unsigned int imguiCategory::SceneGUI::getTexture() {
	return mTexture;
}
const unsigned int imguiCategory::SceneGUI::getRBO() {
	return mRBO;
}

	/*----------------------------------------------------------------------------------*/

	// Console

	void imguiCategory::ConsoleGUI::update() {

		if (::should_console_gui) {
			// Set window size and pos
			ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_FirstUseEver);

			ImGui::Begin("Console##console1");
			{
				// TODO:
			}
			ImGui::End();
		}
	}

	/*----------------------------------------------------------------------------------*/

	// Logger

	char imguiCategory::LoggerGUI::mFind[250];
	bool imguiCategory::LoggerGUI::mInfoCategory = true;
	bool imguiCategory::LoggerGUI::mWarningCategory = true;
	bool imguiCategory::LoggerGUI::mErrorCategory = true;

	void imguiCategory::LoggerGUI::update() {

		if (::should_logger_gui) {
			// Set window size and pos
			ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_FirstUseEver);

			ImGui::Begin("Log##logger1");
			{
				std::deque<GUILogData> gui_log_data = getGUILogData();

				//  Output log onto window
				ImGui::BeginChild("log_text1", ImVec2(0.f, 0.f), false, ImGuiWindowFlags_HorizontalScrollbar);
				{
					for (GUILogData const& log_data : gui_log_data) {
						if (std::count(std::begin(mFind), std::begin(mFind) + strlen(mFind), ' ') == strlen(mFind)) {
							if (mInfoCategory && log_data.type == logType::INFO) {
								ImGui::PushStyleColor(ImGuiCol_Text, setGUILogTextColour(log_data.type));
								ImGui::TextWrapped("%s", log_data.full_log.data());
								ImGui::PopStyleColor();
							}
							if (mWarningCategory && log_data.type == logType::WARNING) {
								ImGui::PushStyleColor(ImGuiCol_Text, setGUILogTextColour(log_data.type));
								ImGui::TextWrapped("%s", log_data.full_log.data());
								ImGui::PopStyleColor();
							}
							if (mErrorCategory && log_data.type == logType::ERROR) {
								ImGui::PushStyleColor(ImGuiCol_Text, setGUILogTextColour(log_data.type));
								ImGui::TextWrapped("%s", log_data.full_log.data());
								ImGui::PopStyleColor();
							}
						}
						// User searching for log
						else {
							if (startsWith(log_data.message, mFind)) {
								ImGui::PushStyleColor(ImGuiCol_Text, setGUILogTextColour(log_data.type));
								ImGui::TextWrapped("%s", log_data.full_log.data());
								ImGui::PopStyleColor();
							}
						}
					}
				}
				ImGui::EndChild();

				//ImGui::

				// Sidebar
				ImGui::BeginChild("log_sidebar1", ImVec2(0.f, 0.f), false);
				{
					// Search
					ImGui::Text("Find: ");
					ImGui::SameLine();
					ImGui::InputText("##find_log1", mFind, IM_ARRAYSIZE(mFind));
					ImGui::Text("\n\n\n");

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 10.f));
					ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
					ImGui::PopStyleVar();

					// Clear & Copy
					if (ImGui::Button(ICON_FA_TRASH_ALT "   Clear ##clear_log1")) {
						clearGUILog();
					}
					if (ImGui::Button(ICON_FA_COPY "   Copy ##copy_log1")) {
						ImGui::LogToClipboard();
						for (GUILogData log_data : gui_log_data) {
							ImGui::LogText(fmt::format("{}\n", log_data.full_log).data());
						}
						ImGui::LogFinish();
					}
					ImGui::Text("\n\n\n");

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 10.f));
					ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
					ImGui::PopStyleVar();

					// Categories
					ImGui::TextWrapped("Categories");
					ImGui::Checkbox("INFO##info_category1", &mInfoCategory);
					ImGui::Checkbox("WARNING##warning_category1", &mWarningCategory);
					ImGui::Checkbox("ERROR##error_category1", &mErrorCategory);
					ImGui::Text("\n\n\n");

					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 10.f));
					ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
					ImGui::PopStyleVar();

					// Dummy log
					if (ImGui::Button(ICON_FA_PLUS "   Dummy Log ##dummy_log1")) {
						for (u8 i = 0; i < 5; i++) {
							u8 ran = NDRNG::intInRange(1, 3);

							switch (ran) {
							case 1:
								log(logType::INFO, "This is information");
								break;
							case 2:
								log(logType::WARNING, "This is a warning");
								break;
							case 3:
								log(logType::ERROR, "This is an error");
								break;
							}
						}
					}
				}
				ImGui::EndChild();

			}
			ImGui::End();
		}
	}

	/*----------------------------------------------------------------------------------*/

	// Profiler

	void imguiCategory::ProfilerGUI::update(Time delta_time) {

		if (::should_profiler_gui) {
			// Set window size and pos
			ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_FirstUseEver);

			ImGui::Begin("Profiler##profiler1");
			{
				// TODO:
				// FPS
				mFPS = getFramesPerSecond(delta_time);
				ImGui::TextWrapped("FPS: %d", mFPS);

			}
			ImGui::End();
		}
	}
	
	/*----------------------------------------------------------------------------------*/


	// Check if mouse is hovering over UI	

	bool shouldIsolate() {
		return ::should_isolate;
	}

	/*----------------------------------------------------------------------------------*/
#endif
