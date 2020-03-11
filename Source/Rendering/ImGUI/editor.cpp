#include "Rendering/ImGUI/editor.hpp"


namespace collapsingHeader {

	/*----------------------------------------------------------------------------------*/

	/*------------------------------------------------------------*/

	const char* TextureUI::mCurrentItem = "None";
	u8 TextureUI::mSelectedValue = 0;
	bool TextureUI::mApplyTexture;
	bool TextureUI::mApplyTransparentOverlay;

#ifdef DEBUG_MODE
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
				if (correct_texture) {
					if (mApplyTexture) {
						loaded_textures[i]->bind(0);
					}
					else {
						error_texture.bind(0);
					}

					break;
				}
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

	ImVec4 ColourUI::colour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
	bool ColourUI::apply_background;

#ifdef DEBUG_MODE
	void ColourUI::display() {
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
	void ColourUI::process() {
		if (apply_background) {
			setBackgroundColour(colour.x, colour.y, colour.z, colour.w);
		}
		else {
			setBackgroundColour(0.2f, 0.3f, 0.3f, 1.0f);
		}
	}

	/*------------------------------------------------------------*/

	bool MiscellaneousUI::mWireframeModeChecked;

#ifdef DEBUG_MODE
	void MiscellaneousUI::display() {
		if (ImGui::CollapsingHeader("Miscellaneous")) {

			// Polygon toggle checkbox: 
			ImGui::TextWrapped("Polygon Toggle: ");
			ImGui::Checkbox("###polygon_mode_checkbox1", &mWireframeModeChecked);
			ImGui::Text("\n");
		}
	}
#endif
	void MiscellaneousUI::process() {
		if (mWireframeModeChecked) {
			wireframe_mode = true;
		}
		else {
			wireframe_mode = false;
		}
	}

	/*----------------------------------------------------------------------------------*/

#ifdef DEBUG_MODE
	void controlsText() {
		if (ImGui::CollapsingHeader("Controls")) {

			ImGui::TextWrapped("- 'W' = Move Forward");
			ImGui::TextWrapped("- 'A' = Move Left");
			ImGui::TextWrapped("- 'S' = Move Backward");
			ImGui::TextWrapped("- 'D' = Move Right");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("- 'Escape' = Quit game");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("- 'Left Shift' = Toggle outline and fill mode");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("- '0 (Zero)' = Hold to disbale all other input methods / controls.");
			ImGui::TextWrapped("\n");
		}
	}

	/*------------------------------------------------------------*/

	void aboutText() {
		if (ImGui::CollapsingHeader("About")) {

			ImGui::TextWrapped("This is a game engine which is currently being developed in C++ and OpenGL.");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("Devloper: Isam Ilyas");
			ImGui::TextWrapped("\n");
			ImGui::TextWrapped("To learn more please visit: ");
			ImGui::TextWrapped("https://github.com/Isam-Ilyas29/OpenGL-Graphics-Engine");
			ImGui::TextWrapped("\n");
		}
	}
#endif

	/*----------------------------------------------------------------------------------*/

} // namespace collapsingHeader

#ifdef DEBUG_MODE
bool isMouseOverUI() {
	if (ImGui::IsWindowHovered()) {
		return true;
	}
	else {
		return false;
	}
}
#endif
