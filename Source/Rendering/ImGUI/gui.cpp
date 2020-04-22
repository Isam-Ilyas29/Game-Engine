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

		bool should_isolate = false;

	} // namespace
#endif

/*----------------------------------------------------------------------------------*/

// Editor

#ifdef IMGUI_LAYER
	// Texture
	const char* imguiCategory::EditorGUI::mCurrentItem = "None";
	u8 imguiCategory::EditorGUI::mSelectedValue = 0;
	bool imguiCategory::EditorGUI::mApplyTexture;
	bool imguiCategory::EditorGUI::mApplyTransparentOverlay;

	// Background Colour

	ImVec4 imguiCategory::EditorGUI::mLocalColour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
	bool imguiCategory::EditorGUI::mApplyBackground;

	// Polygon Mode

	int imguiCategory::EditorGUI::mSelectedItem;
	const char* imguiCategory::EditorGUI::mItems[] = { "Fill", "Line", "Point" };

	void imguiCategory::EditorGUI::process(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture) {
		// Set window size and pos
		ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_Appearing);

		ImGui::Begin("Editor###editor1");
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
				if (ImGui::BeginCombo("###texture_picker1", mCurrentItem, ImGuiComboFlags_NoArrowButton)) {
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
				if (ImGui::ArrowButton("###left1", ImGuiDir_Left)) {
					if (mSelectedValue != 0) {
						mSelectedValue -= 1;
						mCurrentItem = items[mSelectedValue];
					}
				}
				ImGui::SameLine(0, spacing);
				if (ImGui::ArrowButton("###right1", ImGuiDir_Right)) {
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
						ImGui::Checkbox("###apply_texture1", &mApplyTexture);

						break;
					}
				}

				// Transparent Overlay
				ImGui::TextWrapped("\n");
				ImGui::TextWrapped("Transparent Overlay: ");
				ImGui::SameLine();
				ImGui::Checkbox("###transparent_overlay1", &mApplyTransparentOverlay);

				ImGui::Text("\n");
			}

			// Background Colour
			if (ImGui::CollapsingHeader("Background Colour")) {
				ImGui::TextWrapped("\nBackground Colour Picker: ");
				ImGui::ColorEdit3("###background_colour_picker1", (float*)&mLocalColour);

				ImGui::TextWrapped("\n");
				ImGui::TextWrapped("Apply: ");
				ImGui::SameLine();
				ImGui::Checkbox("###apply_background1", &mApplyBackground);

				ImGui::Text("\n");
			}

			// Polygon Mode
			if (ImGui::CollapsingHeader("Polygon Mode")) {

				// Polygon toggle checkbox: 
				ImGui::TextWrapped("\nPolygon Mode: ");
				ImGui::ListBox("###polygon_mode1", &mSelectedItem, mItems, IM_ARRAYSIZE(mItems), 3);
				ImGui::Text("\n");
			}

		}
		ImGui::End();

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
#endif

/*----------------------------------------------------------------------------------*/

// Scene

#ifdef IMGUI_LAYER
	imguiCategory::SceneGUI::SceneGUI() {

		glViewport(0, 0, mWindowSize.x, mWindowSize.y);

		// FBO
		GLAD_CHECK_ERROR(glGenFramebuffers(1, &mFBO));
		GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mFBO));

		// Creates a color attachment texture
		GLAD_CHECK_ERROR(glGenTextures(1, &mTexture));
		GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, mTexture));
		GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWindowSize.x, mWindowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLAD_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0));

		// Create a renderbuffer object for depth and stencil attachment
		GLAD_CHECK_ERROR(glGenRenderbuffers(1, &mRBO));
		GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
		GLAD_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWindowSize.x, mWindowSize.y));
		GLAD_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO));

		// Check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			log(logType::ERROR, "Framebuffer is not complete");
		}

		GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void imguiCategory::SceneGUI::process() {

		// Bind to texture attachment
		GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, mTexture));

		//Scene
		ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_Appearing);
		ImGui::Begin("Scene###scene1", nullptr);
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
			if (isResizing && (ImGui::GetWindowContentRegionWidth() == mPreviousWidth || ImGui::GetWindowContentRegionHeight() == mPreviousHeight)) {
				// Viewport
				glViewport(0, 0, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight());

				// Creates a color attachment texture
				GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, mTexture));
				GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowContentRegionHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
				
				// Creates a renderbuffer object for depth and stencil attachment
				GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mRBO));
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

	const unsigned int imguiCategory::SceneGUI::getFBO() {
		return mFBO;
	}
	const unsigned int imguiCategory::SceneGUI::getTexture() {
		return mTexture;
	}
	const unsigned int imguiCategory::SceneGUI::getRBO() {
		return mRBO;
	}
#endif

/*----------------------------------------------------------------------------------*/

// Logger

#ifdef IMGUI_LAYER
	char imguiCategory::LoggerGUI::mFind[250];
	bool imguiCategory::LoggerGUI::mInfoCategory = true;
	bool imguiCategory::LoggerGUI::mWarningCategory = true;
	bool imguiCategory::LoggerGUI::mErrorCategory = true;

	void imguiCategory::LoggerGUI::process() {
		// Set window size and pos
		ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_Appearing);

		ImGui::Begin("Log###log1");
		{
			ImGui::Columns(2);

			//  Output log onto window
			ImGui::BeginChild("log_text1", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

			std::deque<GUILogData> gui_log_data = getGUILogData();

			for (GUILogData log_data : gui_log_data) {
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

			ImGui::EndChild();

			// Sidebar
			ImGui::NextColumn();
			ImGui::SetColumnOffset(-1, ImGui::GetWindowSize().x * 0.65f);

			ImGui::BeginChild("log_sidebar1", ImVec2(0, 0), false);
			{
				// Search
				ImGui::Text("Find: ");
				ImGui::SameLine();
				ImGui::InputText("###find_log1", mFind, IM_ARRAYSIZE(mFind));
				ImGui::Text("\n\n\n");

				ImGui::HorizontalSeparator();

				// Clear & Copy
				if (ImGui::Button(ICON_FA_TRASH_ALT "   Clear ###clear_log1")) {
					clearGUILog();
				}
				if (ImGui::Button(ICON_FA_COPY "   Copy ###copy_log1")) {
					ImGui::LogToClipboard();
					for (GUILogData log_data : gui_log_data) {
						ImGui::LogText(fmt::format("{}\n", log_data.full_log).data());
					}
					ImGui::LogFinish();
				}
				ImGui::Text("\n\n\n");

				ImGui::HorizontalSeparator();

				// Categories
				ImGui::TextWrapped("Categories");
				ImGui::Checkbox("INFO###info_category1", &mInfoCategory);
				ImGui::Checkbox("WARNING###warning_category1", &mWarningCategory);
				ImGui::Checkbox("ERROR###error_category1", &mErrorCategory);
				ImGui::Text("\n\n\n");

				ImGui::HorizontalSeparator();

				// Dummy log
				if (ImGui::Button(ICON_FA_PLUS "   Dummy Log ###dummy_log1")) {
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
#endif

/*----------------------------------------------------------------------------------*/

// Profiler

#ifdef IMGUI_LAYER
void imguiCategory::ProfilerGUI::process(Time delta_time) {
	// Set window size and pos
	ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_Appearing);

	ImGui::Begin("Profiler###profiler1");
	{
		// TODO:
		// FPS
		mFPS = getFramesPerSecond(delta_time);
		ImGui::TextWrapped("FPS: %d", mFPS);

	}
	ImGui::End();
}
#endif

/*----------------------------------------------------------------------------------*/

// Console

#ifdef IMGUI_LAYER
void imguiCategory::ConsoleGUI::process() {
	// Set window size and pos
	ImGui::SetNextWindowSize(mWindowSize, ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(mWindowPos, ImGuiCond_Appearing);

	ImGui::Begin("Console###console1");
	{
		if (ImGui::IsWindowHovered()) {
			::should_isolate = true;
		}

		// TODO:
	}
	ImGui::End();
}
#endif

/*----------------------------------------------------------------------------------*/

// Text

#ifdef IMGUI_LAYER

#endif

/*----------------------------------------------------------------------------------*/


// Check if mouse is hovering over UI	
#ifdef IMGUI_LAYER
bool shouldIsolate() {
	return ::should_isolate;
}
#endif

/*----------------------------------------------------------------------------------*/
