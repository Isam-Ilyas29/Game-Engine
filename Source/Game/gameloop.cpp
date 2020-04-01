#include "Game/gameloop.hpp"

#include "Core/logger.hpp"
#include "Core/profiler.hpp"
#include "Core/time.hpp"
#include "Core/rng.hpp"
#include "Context/context.hpp"
#include "Rendering/ImGUI/imgui_theme.hpp"
#include "Rendering/graphic.hpp"
#include "Rendering/shader.hpp"
#include "Rendering/texture.hpp"
#include "Rendering/ImGUI/gui.hpp"
#include "Input/input_responder.hpp"
#include "Core/utils.hpp"

#include <fmt/format.h>


bool gameloop::run(int argc, char* argv[]) {

	std::filesystem::path exeFile = argv[0];
	environment::exePath = exeFile.parent_path();
	environment::resourcePath = environment::exePath.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	// Context

	context::initialiseGLFW();

	std::string name = "My Game Engine";  u16 width = 600; u16 height = 450;
	bool success = context::window::setupWindow(name, width, 450);
	if (!success) {
		return false;
	}
	else {
		log(logType::INFO , fmt::format("WINDOW | Window With Properties (\"{}\", {}, {}) Successfully Created", name, width, height));
	}

	success = context::graphics::initialiseGraphics();
	if (!success) {
		return false;
	}

#ifdef IMGUI_LAYER
	context::imguiContext();
	context::setImguiTheme(themeGrey);
	context::setImguiFont(environment::getResourcePath("Fonts/open_sans.ttf"), 17.f);
#endif

	/*----------------------------------------------------------------------------------*/

	{
		Shader shader(environment::getResourcePath("Shaders/shader.vs"), environment::getResourcePath("Shaders/shader.fs"));

		/*----------------------------------------------------------------------------------*/

		// Creating vertex data

		std::array vertices = {
		   -0.5f, -0.5f, -0.5f,  0.f, 0.f,
			0.5f, -0.5f, -0.5f,  1.f, 0.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
		   -0.5f,  0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 0.f,

		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 1.f,
			0.5f,  0.5f,  0.5f,  1.f, 1.f,
		   -0.5f,  0.5f,  0.5f,  0.f, 1.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,

		   -0.5f,  0.5f,  0.5f,  1.f, 0.f,
		   -0.5f,  0.5f, -0.5f,  1.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
		   -0.5f,  0.5f,  0.5f,  1.f, 0.f,

			0.5f,  0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f,  0.5f,  0.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,

		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,
			0.5f, -0.5f, -0.5f,  1.f, 1.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
			0.5f, -0.5f,  0.5f,  1.f, 0.f,
		   -0.5f, -0.5f,  0.5f,  0.f, 0.f,
		   -0.5f, -0.5f, -0.5f,  0.f, 1.f,

		   -0.5f,  0.5f, -0.5f,  0.f, 1.f,
			0.5f,  0.5f, -0.5f,  1.f, 1.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,
			0.5f,  0.5f,  0.5f,  1.f, 0.f,
		   -0.5f,  0.5f,  0.5f,  0.f, 0.f,
		   -0.5f,  0.5f, -0.5f,  0.f, 1.f
		};

		std::array cube_positions = {
			glm::vec3(-2.f, -2.f, -5.f),
			glm::vec3(0.f, -2.f, -5.f),
			glm::vec3(2.f, -2.f, -5.f),
			glm::vec3(-2.f, 0.f, -5.f),
			glm::vec3(0.f, 0.f, -5.f),
			glm::vec3(2.f, 0.f, -5.f),
			glm::vec3(-2.f, 2.f, -5.f),
			glm::vec3(0.f, 2.f, -5.f),
			glm::vec3(2.f, 2.f, -5.f),
		};

		std::vector<unsigned int> indices = {};

		/*----------------------------------------------------------------------------------*/

		// Sending Vertex data to GPU

		unsigned int VBO, VAO;
		GLAD_CHECK_ERROR(glGenBuffers(1, &VBO));
		GLAD_CHECK_ERROR(glGenVertexArrays(1, &VAO));

		VertexData vertex_data;

		vertex_data.setVBO(VBO);
		vertex_data.setVAO(VAO);

		vertex_data.bindVBO(vertices.data(), sizeof(vertices), VBO);
		vertex_data.bindVAO(VAO);

		vertex_data.positionAttrib(0, 5 * sizeof(f32));
		vertex_data.textureAttrib(2, 5 * sizeof(f32));

		/*----------------------------------------------------------------------------------*/
		
		// Textures

		// Dropdown list of .txt file which contains all texture paths
		std::vector<std::string> textures = readFile(environment::getResourcePath("DirectoryReader/textures_list.txt"));

		// Initialise texture objects
		Texture transparent1(environment::getResourcePath("Textures/T_Transparent/graffiti_texture1.png"));

		auto texture1 = std::make_unique<Texture>(environment::getResourcePath("Textures/T_Metal/metal_bricks1.jpg"));
		auto texture2 = std::make_unique<Texture>(environment::getResourcePath("Textures/T_Wood/wood_planks1.jpg"));

		Texture error_texture(environment::getResourcePath("Textures/error_texture1.png"));

		// Add all textures to vector
		std::vector<std::unique_ptr<Texture>> loaded_textures;

		loaded_textures.push_back(NULL);
		loaded_textures.push_back(std::move(texture1));
		loaded_textures.push_back(std::move(texture2));

		// Use shader
		shader.use();
		shader.setInt("transparent1", 1);

		/*----------------------------------------------------------------------------------*/

		// Cubes

		auto transform1 = std::make_unique<Transform>(0, cube_positions.at(0), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform2 = std::make_unique<Transform>(1, cube_positions.at(1), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform3 = std::make_unique<Transform>(2, cube_positions.at(2), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform4 = std::make_unique<Transform>(3, cube_positions.at(3), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform5 = std::make_unique<Transform>(4, cube_positions.at(4), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform6 = std::make_unique<Transform>(5, cube_positions.at(5), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform7 = std::make_unique<Transform>(6, cube_positions.at(6), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform8 = std::make_unique<Transform>(7, cube_positions.at(7), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto transform9 = std::make_unique<Transform>(8, cube_positions.at(8), glm::quat(0.f, 1.0f, 0.3f, 0.5f));

		std::vector<std::unique_ptr<Transform>> cube_position_objects;

		cube_position_objects.push_back(std::move(transform1));
		cube_position_objects.push_back(std::move(transform2));
		cube_position_objects.push_back(std::move(transform3));
		cube_position_objects.push_back(std::move(transform4));
		cube_position_objects.push_back(std::move(transform5));
		cube_position_objects.push_back(std::move(transform6));
		cube_position_objects.push_back(std::move(transform7));
		cube_position_objects.push_back(std::move(transform8));
		cube_position_objects.push_back(std::move(transform9));

		/*----------------------------------------------------------------------------------*/

		Camera camera(-90.f, 0.f, (800.f / 2.f), (600.f / 2.f), 45.f, 100.f, 2.25f);

		Time last_frame = Time::now();

		bool should_isolte = false;


		// GUI

		collapsingHeader::InformationUI information_ui;
		collapsingHeader::LoggerUI logger_ui;
		collapsingHeader::ProfilerUI profiler_ui;
		collapsingHeader::ConsoleUI console_ui;
		collapsingHeader::TextureUI texture_ui;
		collapsingHeader::PolygonModeUI polygon_mode_ui;
		collapsingHeader::BackgroundColourUI colour_ui;

		// Polygon Mode

		std::array polygon_modes = { GL_FILL, GL_LINE, GL_POINT };

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			int pm = polygon_modes[static_cast<u8>(polygon_mode)];
			GLAD_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, pm));

			// Initialise DT
			Time delta_time = Time::now() - last_frame;
			last_frame = Time::now();

			camera.update(delta_time.getSeconds());

#ifdef IMGUI_LAYER
			// Render imGUI

			context::createMainImguiWindow("My GUI###GUI1");

			if (ImGui::BeginTabBar("###tab_bar1")) {

				if (ImGui::BeginTabItem("Debug###debug1")) {
					ImGui::TextWrapped("\n");

					texture_ui.process(std::move(loaded_textures), error_texture, transparent1);
					colour_ui.process();
					polygon_mode_ui.process();

					information_ui.display();
					information_ui.process(delta_time);
					logger_ui.display();
					logger_ui.process();
					profiler_ui.display();
					profiler_ui.process();
					console_ui.display();
					console_ui.process();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Editor###editor1")) {
					ImGui::TextWrapped("\n");

					information_ui.process(delta_time);
					logger_ui.process();
					profiler_ui.process();
					console_ui.process();

					texture_ui.display(textures, std::move(loaded_textures), error_texture, transparent1);
					texture_ui.process(std::move(loaded_textures), error_texture, transparent1);
					colour_ui.display();
					colour_ui.process();
					polygon_mode_ui.display();
					polygon_mode_ui.process();

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Help###help1")) {
					ImGui::TextWrapped("\n");

					information_ui.process(delta_time);
					logger_ui.process();
					profiler_ui.process();
					console_ui.process();
					texture_ui.process(std::move(loaded_textures), error_texture, transparent1);
					colour_ui.process();
					polygon_mode_ui.process();

					collapsingHeader::controlsText();
					collapsingHeader::aboutText();

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			else {
				information_ui.display();
				information_ui.process(delta_time);
				logger_ui.display();
				logger_ui.process();
				profiler_ui.display();
				profiler_ui.process();
				console_ui.display();
				console_ui.process();
				texture_ui.display(textures, std::move(loaded_textures), error_texture, transparent1);
				texture_ui.process(std::move(loaded_textures), error_texture, transparent1);
				colour_ui.display();
				colour_ui.process();
				polygon_mode_ui.display();
				polygon_mode_ui.process();
			}

			should_isolte = isMouseOverGUI();

			ImGui::End();
#else
			texture_ui.process(std::move(loaded_textures), error_texture, transparent1);
			colour_ui.process();
			polygon_mode_ui.process();
#endif

			// Projection + View + Transform [MATRICES]
			glm::mat4 projection = camera.getMat4Projection();
			shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			shader.setMat4("view", view);

			glm::mat4 transform = camera.getMat4Transform();
			s32 transform_loc = glGetUniformLocation(shader.mID, "transform");
			shader.modMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Renders boxes
			for (size_t i = 0; i < cube_position_objects.size(); i++) {
				glm::mat4 model = cube_position_objects[i]->getModel();
				shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glBindVertexArray(VAO));
				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

			// Unbind all texture units at end of frame (helps toggling of textures)
			std::vector<u16> tex_units = { 0, 1, 2 };
			Texture::unbind(tex_units);


#ifdef IMGUI_LAYER
			context::renderImgui();
#endif
			context::window::swapBuffers();
			context::window::pollEvents();
			update(delta_time.getSeconds(), camera, should_isolte);
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}
