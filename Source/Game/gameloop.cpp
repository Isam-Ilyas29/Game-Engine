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

		// Vertex data

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

		// Screen quad

		std::array screen_quad_vertices = { 
		   -1.f,  1.f,  0.f, 1.f,
		   -1.f, -1.f,  0.f, 0.f,
			1.f, -1.f,  1.f, 0.f,

		   -1.f,  1.f,  0.f, 1.f,
			1.f, -1.f,  1.f, 0.f,
			1.f,  1.f,  1.f, 1.f
		};

		/*----------------------------------------------------------------------------------*/

		// Cubes

		unsigned int cube_VBO, cube_VAO;
		GLAD_CHECK_ERROR(glGenBuffers(1, &cube_VBO));
		GLAD_CHECK_ERROR(glGenVertexArrays(1, &cube_VAO));

		GLAD_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, cube_VBO));
		GLAD_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices.data(), GL_STATIC_DRAW));

		GLAD_CHECK_ERROR(glBindVertexArray(cube_VAO));

		//Position attribute
		GLAD_CHECK_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)0));
		GLAD_CHECK_ERROR(glEnableVertexAttribArray(0));
		//Texture co-ordinates attribute
		GLAD_CHECK_ERROR(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*)(3 * sizeof(float))));
		GLAD_CHECK_ERROR(glEnableVertexAttribArray(2));


		// Screen quad

		unsigned int screen_quad_VBO, screen_quad_VAO;
		glGenVertexArrays(1, &screen_quad_VAO);
		glGenBuffers(1, &screen_quad_VBO);
		glBindVertexArray(screen_quad_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, screen_quad_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(screen_quad_vertices), &screen_quad_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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
		main_shader.setInt("transparentTexture1", 1);

		post_processing_shader.use();
		post_processing_shader.setInt("screenTexture", 0);

		/*----------------------------------------------------------------------------------*/

		// Transformations

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

#ifdef IMGUI_LAYER
		// FBO
		unsigned int screen_quad_FBO;
		GLAD_CHECK_ERROR(glGenFramebuffers(1, &screen_quad_FBO));
		GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, screen_quad_FBO));

		// Creates a color attachment texture
		unsigned int screen_quad_texture;
		GLAD_CHECK_ERROR(glGenTextures(1, &screen_quad_texture));
		GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, screen_quad_texture));
		GLAD_CHECK_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1006, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLAD_CHECK_ERROR(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLAD_CHECK_ERROR(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screen_quad_texture, 0));

		// Creates a renderbuffer object for depth and stencil attachment
		unsigned int screen_quad_RBO;
		GLAD_CHECK_ERROR(glGenRenderbuffers(1, &screen_quad_RBO));
		GLAD_CHECK_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, screen_quad_RBO));
		GLAD_CHECK_ERROR(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1006));
		GLAD_CHECK_ERROR(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screen_quad_RBO));

		// Checks if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			log(logType::ERROR, "Framebuffer is not complete");
		}
#endif

		/*----------------------------------------------------------------------------------*/

		// Camera object

		Camera camera(-90.f, 0.f, (800.f / 2.f), (600.f / 2.f), 45.f, 100.f, 2.25f);
		
		/*----------------------------------------------------------------------------------*/

#ifdef IMGUI_LAYER
		// GUI
		imguiCategory::DockspaceAndMenubarGUI dockspace_and_menubar_gui;
		imguiCategory::EditorGUI editor_gui;
		imguiCategory::SceneGUI scene_gui;
		imguiCategory::ConsoleGUI console_gui;
		imguiCategory::LoggerGUI logger_gui;
		imguiCategory::ProfilerGUI profiler_gui;
#endif

		/*----------------------------------------------------------------------------------*/

		// Polygon Mode
		std::array polygon_modes = { GL_FILL, GL_LINE, GL_POINT };

		/*----------------------------------------------------------------------------------*/

		Time last_frame = Time::now();

		bool should_isolte = false;

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			// Initialise DT
			Time delta_time = Time::now() - last_frame;
			last_frame = Time::now();

#ifdef IMGUI_LAYER
			context::beginImguiFrame();

			// GUI
			dockspace_and_menubar_gui.update();
			editor_gui.update(textures, std::move(solid_loaded_textures1), error_texture1, transparent_texture1);
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
			// First pass
			// Clear framebuffers content and enable depth test
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, screen_quad_FBO));
			GLAD_CHECK_ERROR(glClearColor(editor_gui.getBackgroundColour().x, editor_gui.getBackgroundColour().y, editor_gui.getBackgroundColour().z, editor_gui.getBackgroundColour().w));
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GLAD_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
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

			// Polygon Mode
			int pm = polygon_modes[static_cast<u8>(polygon_mode)];
			GLAD_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, pm));

			// Draw boxes
			GLAD_CHECK_ERROR(glBindVertexArray(cube_VAO));

			for (size_t i = 0; i < cube_position_objects.size(); i++) {
				glm::mat4 model = cube_position_objects[i]->getModel();
				main_shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

#ifdef IMGUI_LAYER
			// Second pass
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0)); 
			GLAD_CHECK_ERROR(glClearColor(1.f, 1.f, 1.f, 1.f)); 
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT));

			// Draw post-processed scene on a qaud covering the screen and copy that texture into second FBO
			post_processing_shader.use();
			post_processing_shader.setUint("postProcessingType", editor_gui.getPostProcessingType());
			GLAD_CHECK_ERROR(glBindVertexArray(screen_quad_VAO));
			GLAD_CHECK_ERROR(glDisable(GL_DEPTH_TEST));
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, scene_gui.getFBO()));
			GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, screen_quad_texture));
			GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));

			// GUI
			console_gui.update();
			logger_gui.update();
			profiler_gui.update(delta_time);
			scene_gui.update(screen_quad_texture, screen_quad_RBO);

			// Check whether to default dock
			if (dockspace_and_menubar_gui.getShouldDefaultDock()) {
				dockspace_and_menubar_gui.defaultDock();
			}

			should_isolte = shouldIsolate();
#endif

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
