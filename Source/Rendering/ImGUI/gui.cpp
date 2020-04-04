#include "Rendering/ImGUI/gui.hpp"

#include "Core/utils.hpp"
#include "Core/logger.hpp"
#include "Core/profiler.hpp"
#include "Core/console.hpp"
#include "Context/context.hpp"
#include "Rendering/graphic.hpp"
#include "Core/rng.hpp"

#include "Rendering/ImGUI/imgui_external.hpp"
#include <fmt/format.h>

#include <deque>
#include <algorithm>

#include <iostream> //temp


namespace collapsingHeader {

	/*----------------------------------------------------------------------------------*/

	/*------------------------------------------------------------*/

	// Information

#ifdef IMGUI_LAYER
	void InformationUI::display() {
		if (ImGui::CollapsingHeader("Information")) {

			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("FPS: %d", mFPS);

			ImGui::Text("\n");
		}
	}
#endif
	void InformationUI::process(Time delta_time) {
		mFPS = getFramesPerSecond(delta_time);
	}

	/*------------------------------------------------------------*/

	// Logger

#ifdef IMGUI_LAYER
	char LoggerUI::mFind[250];
	bool LoggerUI::mInfoCategory = true;
	bool LoggerUI::mWarningCategory = true;
	bool LoggerUI::mErrorCategory = true;

	void LoggerUI::display() {
		if (ImGui::CollapsingHeader("Log")) {
			
			ImGui::Text("\n");

			if (ImGui::Button("View log###view_log1")) {
				setGUILogWindow(!isGUILogWindowOpened());
			}

			ImGui::Text("\n");
		}
	}
	void LoggerUI::process() {
		if (isGUILogWindowOpened()) {
			// Set window size and pos
			ImGui::SetNextWindowSize(ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.3476f), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.005), ImGuiCond_Appearing);

			ImGui::Begin("Log###log1");
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

			// Search
			ImGui::Text("Find: ");
			ImGui::SameLine();
			ImGui::InputText("###find_log1", mFind, IM_ARRAYSIZE(mFind));
			ImGui::Text("\n\n\n");

			ImGui::HorizontalSeparator();

			// Clear & Copy
			if (ImGui::Button("Clear###clear_log1")) {
				clearGUILog();
			}
			if (ImGui::Button("Copy###copy_log1")) {
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

			// Temp
			if (ImGui::Button("Dummy Log###dummy_log1")) {
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

			ImGui::EndChild();

			ImGui::End();
		}
	}
#endif

	/*------------------------------------------------------------*/

	// Profiler

#ifdef IMGUI_LAYER
	void ProfilerUI::display() {
		if (ImGui::CollapsingHeader("Profiler")) {

			ImGui::Text("\n");

			if (ImGui::Button("View profiler###view_profiler1")) {
				setProfileWindow(!isProfileWindowOpened());
			}

			ImGui::Text("\n");
		}
	}
	void ProfilerUI::process() {
		if (isProfileWindowOpened()) {
			// Set window size and pos
			ImGui::SetNextWindowSize(ImVec2(context::window::getWidth() * 0.36f, context::window::getHeight() * 0.38f), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(context::window::getWidth() * 0.005f, context::window::getHeight() * 0.6135f), ImGuiCond_Appearing);

			ImGui::Begin("Profiler###profiler1");

			// TODO:

			ImGui::End();
		}
	}
#endif

	/*------------------------------------------------------------*/

	// Console

#ifdef IMGUI_LAYER
	void ConsoleUI::display() {
		if (ImGui::CollapsingHeader("Console")) {

			ImGui::Text("\n");

			if (ImGui::Button("View console###view_console1")) {
				setConsoleWindow(!isConsoleWindowOpened());
			}

			ImGui::Text("\n");
		}
	}
	void ConsoleUI::process() {
		if (isConsoleWindowOpened()) {
			// Set window size and pos
			ImGui::SetNextWindowSize(ImVec2(context::window::getWidth() * 0.34f, context::window::getHeight() * 0.35f), ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(ImVec2(context::window::getWidth() * 0.655f, context::window::getHeight() * 0.6455f), ImGuiCond_Appearing);

			ImGui::Begin("Console###console1");

			// TODO:

			ImGui::End();
		}
	}
#endif

	/*------------------------------------------------------------*/

	// Texture

	const char* TextureUI::mCurrentItem = "None";
	u8 TextureUI::mSelectedValue = 0;
	bool TextureUI::mApplyTexture;
	bool TextureUI::mApplyTransparentOverlay;

#ifdef IMGUI_LAYER
	void TextureUI::display(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture) {
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
	}
#endif
	void TextureUI::process(const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent_texture) {
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
	}

	/*------------------------------------------------------------*/

	// Background colour

	ImVec4 BackgroundColourUI::colour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
	bool BackgroundColourUI::apply_background;

#ifdef IMGUI_LAYER
	void BackgroundColourUI::display() {
		if (ImGui::CollapsingHeader("Colour")) {
			ImGui::TextWrapped("\nBackground Colour Picker: ");
			ImGui::ColorEdit3("###background_colour_picker1", (float*)&colour);

			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("Apply: ");
			ImGui::SameLine();
			ImGui::Checkbox("###apply_background1", &apply_background);

			ImGui::Text("\n");
		}
	}
#endif
	void BackgroundColourUI::process() {
		if (apply_background) {
			setBackgroundColour(colour.x, colour.y, colour.z, colour.w);
		}
		else {
			setBackgroundColour(0.2f, 0.3f, 0.3f, 1.0f);	// Default
		}
	}

	/*------------------------------------------------------------*/

	// Polygon mode

	int PolygonModeUI::mSelectedItem;
	const char* PolygonModeUI::mItems[] = { "Fill", "Line", "Point" };

#ifdef IMGUI_LAYER
	void PolygonModeUI::display() {
		if (ImGui::CollapsingHeader("Miscellaneous")) {

			// Polygon toggle checkbox: 
			ImGui::TextWrapped("\nPolygon Mode: ");
			ImGui::ListBox("###polygon_mode1", &mSelectedItem, mItems, IM_ARRAYSIZE(mItems), 3);
			ImGui::Text("\n");
		}
	}
#endif
	void PolygonModeUI::process() {
		polygon_mode = static_cast<polygonMode>(mSelectedItem);
	}

	/*----------------------------------------------------------------------------------*/

	// Text based UI's

#ifdef IMGUI_LAYER

	/*------------------------------------------------------------*/

	void controlsText() {
		if (ImGui::CollapsingHeader("Controls")) {

			ImGui::TextWrapped("- 'W' = Move Forward");
			ImGui::TextWrapped("- 'A' = Move Left");
			ImGui::TextWrapped("- 'S' = Move Backward");
			ImGui::TextWrapped("- 'D' = Move Right");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("- 'Right Click' = Hold to rotate camera");
			ImGui::TextWrapped("- 'Scroll' = Zoom in and out");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("- 'Escape' = Quit game");
			ImGui::TextWrapped("\n");
		}
	}

	/*------------------------------------------------------------*/

	void aboutText() {
		if (ImGui::CollapsingHeader("About")) {

			ImGui::TextWrapped("This is a game engine which is currently being developed in C++ and OpenGL.");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("Developer: Isam Ilyas");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("To learn more please visit: ");
			ImGui::TextWrapped("https://github.com/Isam-Ilyas29/OpenGL-Graphics-Engine");
			ImGui::TextWrapped("\n");
		}
	}
#endif

	/*----------------------------------------------------------------------------------*/

} // namespace collapsingHeader


// Check if mouse is hovering over UI	
#ifdef IMGUI_LAYER
bool isMouseOverGUI() {
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
		return true;
	}
	else {
		return false;
	}
}
#endif
