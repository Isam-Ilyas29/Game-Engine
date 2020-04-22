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

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <fmt/format.h>

#include <filesystem>
#include <string>
#include <array>
#include <vector>
#include <memory>


bool gameloop::run(int argc, char* argv[]) {

	/*----------------------------------------------------------------------------------*/

	// Environment

	std::filesystem::path exeFile = argv[0];
	environment::exePath = exeFile.parent_path();
	environment::resourcePath = environment::exePath.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	// Context

	context::initialiseGLFW();

	std::string name = "Game Engine";  u16 width = 600; u16 height = 450;
	bool success = context::window::setupWindow(name, width, height);
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
	context::setImguiTextFont(environment::getResourcePath("Fonts/Texts/open_sans.ttf"), 17.f);
	context::setImguiIconFont(environment::getResourcePath("Fonts/Icons/icons_font_awesome.ttf"), 14.5f);
#endif

	/*----------------------------------------------------------------------------------*/

	// Read directories and append file paths to txt

	directoryReader("textures_list.txt", environment::getResourcePath("Textures/Solid"));

	/*----------------------------------------------------------------------------------*/

	{
		Shader main_shader(environment::getResourcePath("Shaders/main.vs"), environment::getResourcePath("Shaders/main.fs"));
		Shader post_processing_shader(environment::getResourcePath("Shaders/post_processing.vs"), environment::getResourcePath("Shaders/post_processing.fs"));

		/*----------------------------------------------------------------------------------*/

		// Creating vertex data

		// Cubes

		std::array cube_vertices = {
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

		// Cubes

		unsigned int cube_VBO, cube_VAO;
		GLAD_CHECK_ERROR(glGenBuffers(1, &cube_VBO));
		GLAD_CHECK_ERROR(glGenVertexArrays(1, &cube_VAO));

		VertexData cube_vertex_data;

		cube_vertex_data.setVBO(cube_VBO);
		cube_vertex_data.setVAO(cube_VAO);

		cube_vertex_data.bindVBO(cube_vertices.data(), sizeof(cube_vertices), cube_VBO);
		cube_vertex_data.bindVAO(cube_VAO);

		cube_vertex_data.positionAttrib(0, 5 * sizeof(f32));
		cube_vertex_data.textureAttrib(2, 5 * sizeof(f32));

		/*----------------------------------------------------------------------------------*/
		
		// Textures

#ifdef IMGUI_LAYER
		// Dropdown list of .txt file which contains all texture paths
		std::vector<std::string> textures = readFile(environment::getResourcePath("DirectoryReader/textures_list.txt"));
#endif

		// Initialise texture objects
		Texture transparent_texture1(environment::getResourcePath("Textures/Transparent/Graffiti/graffiti_texture1.png"));

		auto solid_texture1 = std::make_unique<Texture>(environment::getResourcePath("Textures/Solid/Metal/metal_bricks1.jpg"));
		auto solid_texture2 = std::make_unique<Texture>(environment::getResourcePath("Textures/Solid/Wood/wood_planks1.jpg"));

		Texture error_texture1(environment::getResourcePath("Textures/Other/error_texture1.png"));

#ifdef IMGUI_LAYER
		// Add all textures to vector
		std::vector<std::unique_ptr<Texture>> solid_loaded_textures1;

		solid_loaded_textures1.push_back(NULL);
		solid_loaded_textures1.push_back(std::move(solid_texture1));
		solid_loaded_textures1.push_back(std::move(solid_texture2));
#endif

		/*----------------------------------------------------------------------------------*/

		// Shader
		main_shader.use();
		main_shader.setInt("transparent_texture1", 1);

		post_processing_shader.use();
		post_processing_shader.setInt("screenTexture", 0);

		/*----------------------------------------------------------------------------------*/

		// Cubes

		auto cube_transform1 = std::make_unique<Transform>(0, cube_positions.at(0), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform2 = std::make_unique<Transform>(1, cube_positions.at(1), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform3 = std::make_unique<Transform>(2, cube_positions.at(2), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform4 = std::make_unique<Transform>(3, cube_positions.at(3), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform5 = std::make_unique<Transform>(4, cube_positions.at(4), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform6 = std::make_unique<Transform>(5, cube_positions.at(5), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform7 = std::make_unique<Transform>(6, cube_positions.at(6), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform8 = std::make_unique<Transform>(7, cube_positions.at(7), glm::quat(0.f, 1.0f, 0.3f, 0.5f));
		auto cube_transform9 = std::make_unique<Transform>(8, cube_positions.at(8), glm::quat(0.f, 1.0f, 0.3f, 0.5f));

		std::vector<std::unique_ptr<Transform>> cube_position_objects;

		cube_position_objects.push_back(std::move(cube_transform1));
		cube_position_objects.push_back(std::move(cube_transform2));
		cube_position_objects.push_back(std::move(cube_transform3));
		cube_position_objects.push_back(std::move(cube_transform4));
		cube_position_objects.push_back(std::move(cube_transform5));
		cube_position_objects.push_back(std::move(cube_transform6));
		cube_position_objects.push_back(std::move(cube_transform7));
		cube_position_objects.push_back(std::move(cube_transform8));
		cube_position_objects.push_back(std::move(cube_transform9));

		/*----------------------------------------------------------------------------------*/

		Camera camera(-90.f, 0.f, (800.f / 2.f), (600.f / 2.f), 45.f, 100.f, 2.25f);

		Time last_frame = Time::now();

		bool should_isolte = false;


#ifdef IMGUI_LAYER
		// GUI
		imguiCategory::EditorGUI editor_gui;
		imguiCategory::SceneGUI scene_gui;
		imguiCategory::LoggerGUI logger_gui;
		imguiCategory::ProfilerGUI profiler_gui;
		imguiCategory::ConsoleGUI console_gui;
#endif

		// Polygon Mode

		std::array polygon_modes = { GL_FILL, GL_LINE, GL_POINT };

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			// Initialise DT
			Time delta_time = Time::now() - last_frame;
			last_frame = Time::now();

#ifdef IMGUI_LAYER
			context::beginImguiFrame();

			// Editor GUI
			editor_gui.process(textures, std::move(solid_loaded_textures1), error_texture1, transparent_texture1);
#else
			// Background colour
			GLAD_CHECK_ERROR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			// Textures
			solid_texture1->bind(0);
			transparent_texture1.bind(1);

			// Polygon Mode
			polygon_mode = polygonMode::FILL;
#endif

			camera.update(delta_time.getSeconds());

#ifdef IMGUI_LAYER
			// Bind FBO & Enable depth test
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, scene_gui.getFBO()));
			GLAD_CHECK_ERROR(glEnable(GL_DEPTH_TEST));

			// Clear framebuffers content
			glClearColor(editor_gui.getBackgroundColour().x, editor_gui.getBackgroundColour().y, editor_gui.getBackgroundColour().z, editor_gui.getBackgroundColour().w);
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
#endif

			// Projection + View + Transform [MATRICES]
			main_shader.use();

			glm::mat4 projection = camera.getMat4Projection();
			main_shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			main_shader.setMat4("view", view);

			glm::mat4 transform = camera.getMat4Transform();
			s32 transform_loc = glGetUniformLocation(main_shader.mID, "transform");
			main_shader.modMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Draw boxes
			GLAD_CHECK_ERROR(glBindVertexArray(cube_VAO));

			for (size_t i = 0; i < cube_position_objects.size(); i++) {
				glm::mat4 model = cube_position_objects[i]->getModel();
				main_shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

#ifdef IMGUI_LAYER
			// GUI 
			logger_gui.process();
			profiler_gui.process(delta_time);
			console_gui.process();

			// Bind back to default framebuffer and draw quad with attached framebuffer colour texture
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));
			GLAD_CHECK_ERROR(glDisable(GL_DEPTH_TEST));
			// Clear framebuffers contents
			GLAD_CHECK_ERROR(glClearColor(1.f, 1.f, 1.f, 1.f));
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT));
			scene_gui.process();

			should_isolte = shouldIsolate();
#endif

			// Polygon Mode
			int pm = polygon_modes[static_cast<u8>(polygon_mode)];
			GLAD_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, pm));

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
