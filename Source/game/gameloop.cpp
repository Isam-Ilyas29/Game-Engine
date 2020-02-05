#include "gameloop.h"

#include "../Context/context.h"
#include "../Rendering/graphic.h"
#include "../Rendering/shader.h"
#include "../Rendering/texture.h"
#include "../Input/input_responder.h"
#include "../Tools/tool.h"
#include "../Rendering/ImGUI/utils.h"



bool gameloop::run(int argc, char* argv[]) {

	std::filesystem::path exeFile = argv[0];
	environment::exeDirectory = exeFile.parent_path();
	environment::resourcesPath = environment::exeDirectory.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	context::initialiseGLFW();

	bool success = context::window::setupWindow("My Game", 600, 450);
	if (!success) {
		return false;
	}

	success = context::graphics::initialiseGraphics();
	if (!success) {
		return false;
	}

	context::imguiContext();

	/*----------------------------------------------------------------------------------*/

	{
		Shader shader(environment::ResourcePath("shader.vs"), environment::ResourcePath("shader.fs"));

		float vertices[] = {
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glm::vec3 cube_positions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};

		std::vector<unsigned int> indices = {};

		/*----------------------------------------------------------------------------------*/

		unsigned int VBO, VAO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		VertAttribObject vertex_and_attrib_object(VAO, VBO);

		vertex_and_attrib_object.bindVBO(vertices, sizeof(vertices), VBO);
		vertex_and_attrib_object.bindVAO(VAO);

		vertex_and_attrib_object.positionAttrib();
		vertex_and_attrib_object.textureCoordAttrib();

		/*----------------------------------------------------------------------------------*/

		std::vector<std::string> textures = readFile("../Resources/DirectoryReader/textures_list.txt");

		/*----------------------------------------------------------------------------------*/

		Texture transparent1(environment::ResourcePath("Textures/TransparentTextures/graffiti_texture1.png"));

		auto texture1 = std::make_unique<Texture>(environment::ResourcePath("Textures/MetalTextures/metal_bricks1.jpg"));
		auto texture2 = std::make_unique<Texture>(environment::ResourcePath("Textures/WoodTextures/wood_planks1.jpg"));
		auto texture3 = std::make_unique<Texture>(environment::ResourcePath("Textures/BrickTextures/white_brick_wall1.jpg"));

		std::vector<std::unique_ptr<Texture>> loaded_textures;

		loaded_textures.push_back(std::move(texture1));
		loaded_textures.push_back(std::move(texture2));
		loaded_textures.push_back(std::move(texture3));

		shader.use();
		shader.setInt("transparent1", 1);

		/*----------------------------------------------------------------------------------*/

		float time = 2.0f;

		/*----------------------------------------------------------------------------------*/

		Camera camera(true, -90.0f, 0.0f, (800.0f / 2.0f), (600.0f / 2.0f), 45.0f, 100.0f);

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			glPolygonMode(GL_FRONT_AND_BACK, polygon_mode ? GL_LINE : GL_FILL);

			camera.perFrameTimeLogic();

			// Displays FPS every 2 seconds
			time -= delta_time;
			if (time <= 0.0f) {
				time = 2.0f;
				std::cout << "FPS: " << framesPerSecond() << std::endl;
			}

			screenColour(0.2, 0.3, 0.3, 1.0);

			// Render imGUI
			createImguiWindow("My GUI###GUI1");

			// Polygon toggle checkbox: 
			CreateCheckbox polygon_mode_checkbox("Polygon Toggle: ", "###polygon_mode_checkbox1");
			if (polygon_mode_checkbox.isChecked()) {
				polygon_mode = true;
			}
			else {
				polygon_mode = false;
			}

			// Texture loader combo:
			CreateCombo texture_picker("\nTexture Picker: ", "###texture_picker1", textures, 5);
			int selected_value = texture_picker.getSelectedItem();
			std::string selected_item = textures[selected_value];
			//std::cout << selected_value << std::endl;
			//std::cout << selected_item << std::endl;

			for (size_t i = 0; i < loaded_textures.size(); i++) {
				bool correct_texture = (i == selected_value);
				loaded_textures[i]->setTexture(correct_texture, 0);

				if (correct_texture) {
					GLuint imgui_preview_image_texture = 0;

					bool image = loaded_textures[i]->previewImage(&imgui_preview_image_texture);
					IM_ASSERT(image);

					ImGui::Text("\nImage Preview: ");
					ImGui::Image((void*)(intptr_t)imgui_preview_image_texture, ImVec2(160, 160));
				}
			}

			transparent1.setTexture(true, 1);

			ImGui::End();

			// Projection + View + Transform
			glm::mat4 projection = camera.getMat4Projection();
			shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			shader.setMat4("view", view);

			glm::mat4 transform = camera.getMat4Transform();
			unsigned int transform_loc = glGetUniformLocation(shader.mID, "transform");
			shader.modMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Renders boxes
			glBindVertexArray(VAO);
			for (unsigned int i = 0; i < 10; i++) {
				glm::mat4 model = getMat4Model(i, cube_positions);

				shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			context::renderImgui();
			context::window::pollEvents();
			update(delta_time, camera, polygon_mode_checkbox);
			context::window::swapBuffers();
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}

