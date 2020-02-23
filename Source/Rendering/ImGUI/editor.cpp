#include "editor.h"


// Note: this file may seem messy but ensure you read through comments thoroughly to grasp on whats going on

#ifdef DEBUG_MODE

	namespace collapsingHeader {

		void texture(const std::vector<std::string>& textures, const std::vector<std::unique_ptr<Texture>>& loaded_textures, const Texture& error_texture, const Texture& transparent1, bool editor) {

			static const char* current_item = "None";
			static int selected_value = 0;
			static bool apply_texture;
			static bool graffiti_texture;

			// If function call specified editor mode then it will display the collapsing header, else it will show current state but no option to edit over it

			if (editor) {
				if (ImGui::CollapsingHeader("Textures")) {

					// Copes items of passed in vector into another vector of 'const char*'
					std::vector<const char*> items;
					items.push_back("None");
					for (size_t i = 0; i < textures.size(); i++) {
						items.push_back(textures[i].data());
					}

					// Texture loader combo (custom design):
					ImGui::TextWrapped("\nTexture Picker: ");
					if (ImGui::BeginCombo("###texture_picker1", current_item, ImGuiComboFlags_NoArrowButton)) {
						for (int i1 = 0; i1 < items.size(); i1++) {
							bool is_selected = (current_item == items[i1]);
							if (ImGui::Selectable(items[i1], is_selected)) {
								selected_value = i1;		//Index of selected item
								current_item = items[i1];	//Name of selected item
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
						if (selected_value != 0) {
							selected_value -= 1;
							current_item = items[selected_value];
						}
					}
					ImGui::SameLine(0, spacing);
					if (ImGui::ArrowButton("###right1", ImGuiDir_Right)) {
						if (selected_value != (items.size() - 1)) {
							selected_value += 1;
							current_item = items[selected_value];
						}
					}

					for (size_t i2 = 0; i2 < loaded_textures.size(); i2++) {
						bool correct_texture = (i2 == selected_value);

						// Makes sure if selected item is none no image preview will be loaded and in else statement it default loads the error texture
						if (correct_texture && selected_value != 0) {
							// Image previewer
							GLuint imgui_preview_image_texture = 0;

							bool image = loaded_textures[i2 ]->previewImage(&imgui_preview_image_texture);
							IM_ASSERT(image);

							ImGui::TextWrapped("\nImage Preview: ");
							ImGui::Image((void*)(intptr_t)imgui_preview_image_texture, ImVec2(160, 160));

							// Load texture

							ImGui::TextWrapped("\n");
							ImGui::TextWrapped("Apply: ");
							ImGui::SameLine();
							ImGui::Checkbox("###apply_texture1", &apply_texture);

							if (apply_texture) {
								loaded_textures[i2]->bind(0);
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

					// Graffiti toggle
					ImGui::TextWrapped("\n");
					ImGui::TextWrapped("Graffiti: ");
					ImGui::SameLine();
					ImGui::Checkbox("###graffiti_texture1", &graffiti_texture);
					if (graffiti_texture) {
						transparent1.bind(1);
					}

					ImGui::Text("\n");
				}
				else {

					// Texture loader combo:
					for (size_t i2 = 0; i2 < loaded_textures.size(); i2++) {
						bool correct_texture = (i2 == selected_value);

						if (correct_texture) {
							if (apply_texture) {
								loaded_textures[i2]->bind(0);
							}
							else {
								error_texture.bind(0);
							}

							break;
						}
					}

					// Graffiti toggle
					if (graffiti_texture) {
						transparent1.bind(1);
					}
				}
			}
			else {
				// Texture loader combo (Keeps all current data to be processed as combo is not visible on screen [same is done when collapsing header is closed]):
				for (size_t i = 0; i < loaded_textures.size(); i++) {
					bool correct_texture = (i == selected_value);

					if (correct_texture) {
						if (apply_texture) {
							loaded_textures[i]->bind(0);
						}
						else {
							error_texture.bind(0);
						}

						break;
					}
				}

				// Graffiti toggle
				if (graffiti_texture) {
					transparent1.bind(1);
				}
			}
		}


		void colour(bool editor) {

			static ImVec4 colour = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
			static bool apply_background;

			// If function call specified editor mode then it will display the collapsing header, else it will show current state but no option to edit over it

			if (editor) {
				if (ImGui::CollapsingHeader("Colour")) {

					// Background colour picker
					ImGui::TextWrapped("\nBackground Colour Picker: ");
					ImGui::ColorEdit3("###background_colour_picker1", (float*)&colour);

					ImGui::TextWrapped("\n");
					ImGui::TextWrapped("Apply: ");
					ImGui::SameLine();
					ImGui::Checkbox("###apply_background1", &apply_background);

					if (apply_background) {
						setBackgroundColour(colour.x, colour.y, colour.z, colour.w);
					}
					else {
						setBackgroundColour(0.2f, 0.3f, 0.3f, 1.0f);
					}

					ImGui::Text("\n");
				}
				else {

					// Background colour picker
					if (apply_background) {
						setBackgroundColour(colour.x, colour.y, colour.z, colour.w);
					}
					else {
						setBackgroundColour(0.2f, 0.3f, 0.3f, 1.0f);
					}
				}
			}
			else {

				// Background colour picker (Keeps all current data to be processed as colour picker is not visible on screen [same is done when collapsing header is closed])
				if (apply_background) {
					setBackgroundColour(colour.x, colour.y, colour.z, colour.w);
				}
				else {
					setBackgroundColour(0.2f, 0.3f, 0.3f, 1.0f);
				}
			}
		}


		void miscellaneous(bool editor) {

			static bool polygon_mode_checked;

			// If function call specified editor mode then it will display the collapsing header, else it will show current state but no option to edit over it

			if (editor) {
				if (ImGui::CollapsingHeader("Miscellaneous")) {

					// Polygon toggle checkbox: 
					ImGui::TextWrapped("Polygon Toggle: ");
					ImGui::Checkbox("###polygon_mode_checkbox1", &polygon_mode_checked);

					if (polygon_mode_checked) {
						polygon_mode = true;
					}
					else {
						polygon_mode = false;
					}

					ImGui::Text("\n");
				}
				else {

					// Polygon toggle checkbox: 
					if (polygon_mode_checked) {
						polygon_mode = true;
					}
					else {
						polygon_mode = false;
					}
				}
			}
			else {

				// Polygon toggle checkbox (Keeps all current data to be processed as checkbox is not visible on screen) [same is done when collapsing header is closed]: 
				if (polygon_mode_checked) {
					polygon_mode = true;
				}
				else {
					polygon_mode = false;
				}
			}

		}

		/*----------------------------------------------------------------------------------*/

		void controlsText(bool help) {
		
			// If function call specified help mode then it will display the collapsing header, else pass

			if (help) {
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
		}

		void aboutText(bool help) {

			// If function call specified help mode then it will display the collapsing header, else pass

			if (help) {
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
		}
	} // namespace collapsingHeader

	bool isMouseOverUI() {
		if (ImGui::IsWindowHovered()) {
			return true;
		}
		else {
			return false;
		}
	}

#endif


