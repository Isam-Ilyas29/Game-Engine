#include "Game/gameloop.hpp"

#include "Core/logger.hpp"
#include "Core/profiler.hpp"
#include "Core/time.hpp"
#include "Core/rng.hpp"
#include "Environment/environment.hpp"
#include "Context/context.hpp"
#include "Rendering/graphic.hpp"
#include "Rendering/geometry.hpp"
#include "Rendering/mesh.hpp"
#include "Rendering/transform.hpp"
#include "Rendering/shader.hpp"
#include "Rendering/texture.hpp"
#include "Camera/camera.hpp"
#include "Rendering/ImGUI/gui.hpp"
#include "Rendering/ImGUI/imgui_theme.hpp"
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
#include <optional>


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
		log(LogType::INFO, fmt::format("WINDOW | Window With Properties (\"{}\", {}, {}) Successfully Created", name, width, height));
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

	{
		Shader main_shader(environment::getResourcePath("Shaders/main.vs"), environment::getResourcePath("Shaders/main.fs"));

		Shader post_processing_shader(environment::getResourcePath("Shaders/post_processing.vs"), environment::getResourcePath("Shaders/post_processing.fs"));

		/*----------------------------------------------------------------------------------*/

		// Screen quad

		std::array screen_quad_vertices = {
		   -1.f,  1.f,  0.f, 1.f,
		   -1.f, -1.f,  0.f, 0.f,
			1.f, -1.f,  1.f, 0.f,

		   -1.f,  1.f,  0.f, 1.f,
			1.f, -1.f,  1.f, 0.f,
			1.f,  1.f,  1.f, 1.f
		};

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

		// Plane mesh

		Geometry plane_geometry;
		plane_geometry.addVertices({
			{ glm::vec3(9.0f, -0.5f,  9.0f),  glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(2.0f, 0.0f) },
			{ glm::vec3(-9.0f, -0.5f,  9.0f), glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3(-9.0f, -0.5f, -9.0f), glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(0.0f, 2.0f) },
			{ glm::vec3(9.0f, -0.5f,  9.0f),  glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(2.0f, 0.0f) },
			{ glm::vec3(-9.0f, -0.5f, -9.0f), glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(0.0f, 2.0f) },
			{ glm::vec3(9.0f, -0.5f, -9.0f),  glm::vec3(0.44f, 0.47f, 0.47f), glm::vec2(2.0f, 2.0f) },

			});

		Material plane_material;

		createMesh("plane_mesh1", plane_geometry, plane_material);

		// Cube positions

		std::array plane_positions = {
			glm::vec3(-0.f, -5.f, 4.f),
		};

		std::vector<std::unique_ptr<Transform>> plane_position_objects;

		for (size_t i = 0; i < plane_positions.size(); i++) {
			plane_position_objects.push_back(std::make_unique<Transform>(0, plane_positions.at(i), glm::quat(1.f, 0.f, 0.f, 0.f)));
		}

		/*----------------------------------------------------------------------------------*/

		// Cube mesh

		Geometry wall_geometry;
		wall_geometry.addVertices({
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 0.f) },

			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 0.f) },

			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 0.f) },

			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.55f, 0.58f, 0.59f), glm::vec2(1.f, 0.f) },

			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 1.f) },

			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.33f, 0.4f, 0.39f), glm::vec2(0.f, 1.f) }
			});

		Material wall_material;

		createMesh("wall_mesh1", wall_geometry, wall_material);

		// Cube positions

		std::array wall_positions = {
			glm::vec3(-9.f, -5.f, -5.f),
			glm::vec3(-8.f, -5.f, -5.f),
			glm::vec3(-7.f, -5.f, -5.f),
			glm::vec3(-6.f, -5.f, -5.f),
			glm::vec3(-5.f, -5.f, -5.f),
			glm::vec3(-4.f, -5.f, -5.f),
			glm::vec3(-3.f, -5.f, -5.f),
			glm::vec3(-2.f, -5.f, -5.f),
			glm::vec3(-1.f, -5.f, -5.f),
			glm::vec3(0.f, -5.f, -5.f),
			glm::vec3(1.f, -5.f, -5.f),
			glm::vec3(2.f, -5.f, -5.f),
			glm::vec3(3.f, -5.f, -5.f),
			glm::vec3(4.f, -5.f, -5.f),
			glm::vec3(5.f, -5.f, -5.f),
			glm::vec3(6.f, -5.f, -5.f),
			glm::vec3(7.f, -5.f, -5.f),
			glm::vec3(8.f, -5.f, -5.f),
			glm::vec3(9.f, -5.f, -5.f),

			glm::vec3(9.f, -5.f, -4.f),
			glm::vec3(9.f, -5.f, -3.f),
			glm::vec3(9.f, -5.f, -2.f),
			glm::vec3(9.f, -5.f, -1.f),
			glm::vec3(9.f, -5.f, 0.f),
			glm::vec3(9.f, -5.f, 1.f),
			glm::vec3(9.f, -5.f, 2.f),
			glm::vec3(9.f, -5.f, 3.f),
			glm::vec3(9.f, -5.f, 4.f),
			glm::vec3(9.f, -5.f, 5.f),
			glm::vec3(9.f, -5.f, 6.f),
			glm::vec3(9.f, -5.f, 7.f),
			glm::vec3(9.f, -5.f, 8.f),
			glm::vec3(9.f, -5.f, 9.f),
			glm::vec3(9.f, -5.f, 10.f),
			glm::vec3(9.f, -5.f, 11.f),
			glm::vec3(9.f, -5.f, 12.f),
			glm::vec3(9.f, -5.f, 13.f),

			glm::vec3(-9.f, -5.f, 13.f),
			glm::vec3(-8.f, -5.f, 13.f),
			glm::vec3(-7.f, -5.f, 13.f),
			glm::vec3(-6.f, -5.f, 13.f),
			glm::vec3(-5.f, -5.f, 13.f),
			glm::vec3(-4.f, -5.f, 13.f),
			glm::vec3(-3.f, -5.f, 13.f),
			glm::vec3(-2.f, -5.f, 13.f),
			glm::vec3(-1.f, -5.f, 13.f),
			glm::vec3(0.f, -5.f, 13.f),
			glm::vec3(1.f, -5.f, 13.f),
			glm::vec3(2.f, -5.f, 13.f),
			glm::vec3(3.f, -5.f, 13.f),
			glm::vec3(4.f, -5.f, 13.f),
			glm::vec3(5.f, -5.f, 13.f),
			glm::vec3(6.f, -5.f, 13.f),
			glm::vec3(7.f, -5.f, 13.f),
			glm::vec3(8.f, -5.f, 13.f),

			glm::vec3(-9.f, -5.f, -5.f),
			glm::vec3(-9.f, -5.f, -4.f),
			glm::vec3(-9.f, -5.f, -3.f),
			glm::vec3(-9.f, -5.f, -2.f),
			glm::vec3(-9.f, -5.f, -1.f),
			glm::vec3(-9.f, -5.f, 0.f),
			glm::vec3(-9.f, -5.f, 1.f),
			glm::vec3(-9.f, -5.f, 2.f),
			glm::vec3(-9.f, -5.f, 3.f),
			glm::vec3(-9.f, -5.f, 4.f),
			glm::vec3(-9.f, -5.f, 5.f),
			glm::vec3(-9.f, -5.f, 6.f),
			glm::vec3(-9.f, -5.f, 7.f),
			glm::vec3(-9.f, -5.f, 8.f),
			glm::vec3(-9.f, -5.f, 9.f),
			glm::vec3(-9.f, -5.f, 10.f),
			glm::vec3(-9.f, -5.f, 11.f),
			glm::vec3(-9.f, -5.f, 12.f),
			glm::vec3(-9.f, -5.f, 13.f),
		};

		std::vector<std::unique_ptr<Transform>> wall_position_objects;

		for (size_t i = 0; i < wall_positions.size(); i++) {
			wall_position_objects.push_back(std::make_unique<Transform>(0, wall_positions.at(i), glm::quat(1.f, 0.f, 0.f, 0.f)));
		}

		/*----------------------------------------------------------------------------------*/

		// Snake mesh

		Geometry snake_geometry;
		snake_geometry.addVertices( {
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 0.f) },
						
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 0.f) },
														
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 0.f) },
													
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.18f, 0.56f, 0.34f), glm::vec2(1.f, 0.f) },

			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 1.f) },
													  			  			  
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.09f, 0.47f, 0.2f),  glm::vec2(0.f, 1.f) }
		} );

		Material snake_material;

		createMesh("snake_mesh1", snake_geometry, snake_material);

		// Snake positions

		std::vector snake_positions = {
			glm::vec3(-0.f, -5.f, 4.f),
		};

		std::vector<std::unique_ptr<Transform>> snake_position_objects;

		for (size_t i = 0; i < snake_positions.size(); i++) {
			snake_position_objects.push_back(std::make_unique<Transform>(0, snake_positions[i], glm::quat(1.f, 0.f, 0.f, 0.f)));
		}

		/*----------------------------------------------------------------------------------*/

		// Apple mesh

		Geometry apple_geometry;
		apple_geometry.addVertices({
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 0.f) },
															
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 0.f) },
														
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 0.f) },
													
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.96f, 0.09f, 0.09f), glm::vec2(1.f, 0.f) },

			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 1.f) },

			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 1.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(1.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 0.f) },
			{ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.89f, 0.03f, 0.03f), glm::vec2(0.f, 1.f) }
			});

		Material apple_material;

		createMesh("apple_mesh1", apple_geometry, apple_material);

		// Snake positions

		std::array apple_positions = {
			glm::vec3(NDRNG::intInRange(-8, 8), -5.f, NDRNG::intInRange(-4, 12)),
		};

		std::vector<std::unique_ptr<Transform>> apple_position_objects;

		for (size_t i = 0; i < apple_positions.size(); i++) {
			apple_position_objects.push_back(std::make_unique<Transform>(0, apple_positions.at(i), glm::quat(1.f, 0.f, 0.f, 0.f)));
		}

		/*----------------------------------------------------------------------------------*/


		// Shader
		main_shader.use();

		post_processing_shader.use();
		post_processing_shader.setInt("screen_texture", 0);

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
			log(LogType::ERROR, "Framebuffer is not complete");
		}
#endif

		/*----------------------------------------------------------------------------------*/

		// Camera 

		Camera camera(-90.f, -56.f, (800.f / 2.f), (600.f / 2.f), 45.f, 100.f, 2.25f);
		camera(glm::vec3(0.185f, 16.85f, 20.18f), glm::vec3(0.f, -0.82f, -0.55f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));

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

		Time last_frame = Time::now();

		bool should_isolte = false;

		/*----------------------------------------------------------------------------------*/

		// 2d array of mesh positions
		auto mesh_positions = std::vector(19, std::vector(19, 'e'));

		// Speed
		f32 snake_speed = 0.41f;

		// Direction
		char direction;

		// Timer
		Time timer;

		// Dead
		bool alive = true;

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (!context::window::isClosed(context::window::getWindow()) && alive) {

			// Initialise DT
			Time delta_time = Time::now() - last_frame;

			// Timer
			timer += Time::now() - last_frame;
			last_frame = Time::now();

			// Adjust mesh positions accordingly
			for (u8 i = 0; i < 18; i++) {
				for (u8 j = 0; j < 18; j++) {
					mesh_positions[i][j] = 'e';
				}
			}

			for (size_t i = 0; i < wall_positions.size(); i++) {
				mesh_positions[static_cast<int>(wall_positions[i].x) + 9][static_cast<int>(wall_positions[i].z) + 5] = 'x';
			}
			for (size_t i = 0; i < apple_positions.size(); i++) {
				mesh_positions[static_cast<int>(apple_positions[i].x) + 9][static_cast<int>(apple_positions[i].z) + 5] = 'a';
			}
			if (mesh_positions[static_cast<int>(snake_positions[0].x) + 9][static_cast<int>(snake_positions[0].z) + 5] == 'x') {
				alive = false;
			}
			else if (mesh_positions[static_cast<int>(snake_positions[0].x) + 9][static_cast<int>(snake_positions[0].z) + 5] == 'a') {
				if (direction == 'u') {
					snake_positions.push_back(glm::vec3(snake_positions[snake_positions.size() - 1].x, -5.f, snake_positions[snake_positions.size() - 1].z - 1.f));
				}
				else if (direction == 'l') {
					snake_positions.push_back(glm::vec3(snake_positions[snake_positions.size() - 1].x - 1.f, -5.f, snake_positions[snake_positions.size() - 1].z));
				}
				else if (direction == 'd') {
					snake_positions.push_back(glm::vec3(snake_positions[snake_positions.size() - 1].x, -5.f, snake_positions[snake_positions.size() - 1].z + 1.f));
				}
				else if (direction == 'r') {
					snake_positions.push_back(glm::vec3(snake_positions[snake_positions.size() - 1].x + 1.f, -5.f, snake_positions[snake_positions.size() - 1].z));
				}

				snake_position_objects.push_back(std::make_unique<Transform>(0, snake_positions.at(snake_positions.size() - 1), glm::quat(1.f, 0.f, 0.f, 0.f)));

				// Create new apple
				apple_positions[0] = glm::vec3(NDRNG::intInRange(-8, 8), -5.f, NDRNG::intInRange(-4, 12));
				apple_position_objects.pop_back();
				apple_position_objects.push_back(std::make_unique<Transform>(0, apple_positions.at(apple_positions.size() - 1), glm::quat(1.f, 0.f, 0.f, 0.f)));
			}
			for (size_t i = 0; i < snake_positions.size(); i++) {
				mesh_positions[static_cast<int>(snake_positions[i].x) + 9][static_cast<int>(snake_positions[i].z) + 5] = 's';
			}

			// Movement
			if (timer >= Time::seconds(snake_speed)) {
				for (size_t i = 1; i < snake_position_objects.size(); i++) {
					if (direction == 'u') {
						snake_positions[i].x = snake_positions[i - 1].x;
						snake_positions[i].z = snake_positions[i - 1].z;
						snake_position_objects[i]->setPosition(snake_positions[i]);
						snake_position_objects[i]->createModel();
					}
					else if (direction == 'l') {
						snake_positions[i].x = snake_positions[i - 1].x;
						snake_positions[i].z = snake_positions[i - 1].z;
						snake_position_objects[i]->setPosition(snake_positions[i]);
						snake_position_objects[i]->createModel();
					}
					else if (direction == 'd') {
						snake_positions[i].x = snake_positions[i - 1].x;
						snake_positions[i].z = snake_positions[i - 1].z;
						snake_position_objects[i]->setPosition(snake_positions[i]);
						snake_position_objects[i]->createModel();
					}
					else if (direction == 'r') {
						snake_positions[i].x = snake_positions[i - 1].x;
						snake_positions[i].z = snake_positions[i - 1].z;
						snake_position_objects[i]->setPosition(snake_positions[i]);
						snake_position_objects[i]->createModel();
					}
				}

				if (direction == 'u') {
					snake_positions[0].z -= 1.f;
					snake_position_objects[0]->setPosition(snake_positions[0]);
					snake_position_objects[0]->createModel();
				}
				else if (direction == 'l') {
					snake_positions[0].x -= 1.f;
					snake_position_objects[0]->setPosition(snake_positions[0]);
					snake_position_objects[0]->createModel();
				}
				else if (direction == 'd') {
					snake_positions[0].z += 1.f;
					snake_position_objects[0]->setPosition(snake_positions[0]);
					snake_position_objects[0]->createModel();
				}
				else if (direction == 'r') {
					snake_positions[0].x += 1.f;
					snake_position_objects[0]->setPosition(snake_positions[0]);
					snake_position_objects[0]->createModel();
				}

				timer = { };
			}

#ifdef IMGUI_LAYER
			context::beginImguiFrame();

			// GUI
			dockspace_and_menubar_gui.update();
			editor_gui.update();
#endif
			// Background colour
			GLAD_CHECK_ERROR(glClearColor(0.2f, 0.3f, 0.3f, 1.f));
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			// Camera
			camera.update(delta_time.getSeconds());

#ifdef IMGUI_LAYER
			// First pass
			// Clear framebuffers content and enable depth test
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, screen_quad_FBO));
			GLAD_CHECK_ERROR(glClearColor(0.2f, 0.3f, 0.3f, 1.f));
			GLAD_CHECK_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
			GLAD_CHECK_ERROR(glEnable(GL_DEPTH_TEST));
#endif
			// Projection + View + Transform [MATRICES]

			main_shader.use();

			glm::mat4 projection = camera.getMat4Projection();
			main_shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			main_shader.setMat4("view", view);

			// Polygon Mode
			GLAD_CHECK_ERROR(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

			// Draw

			// Plane
			GLAD_CHECK_ERROR(glBindVertexArray(findMesh("plane_mesh1").value().vao));

			for (size_t i = 0; i < plane_position_objects.size(); i++) {
				glm::mat4 model = plane_position_objects[i]->getModel();
				main_shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

			// Wall
			GLAD_CHECK_ERROR(glBindVertexArray(findMesh("wall_mesh1").value().vao));

			for (size_t i = 0; i < wall_position_objects.size(); i++) {
				glm::mat4 model = wall_position_objects[i]->getModel();
				main_shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

			// Snake
			GLAD_CHECK_ERROR(glBindVertexArray(findMesh("snake_mesh1").value().vao));

			for (size_t i = 0; i < snake_position_objects.size(); i++) {
				glm::mat4 model = snake_position_objects[i]->getModel();
				main_shader.setMat4("model", model);

				GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

			// Apple
			GLAD_CHECK_ERROR(glBindVertexArray(findMesh("apple_mesh1").value().vao));

			for (size_t i = 0; i < apple_position_objects.size(); i++) {
				glm::mat4 model = apple_position_objects[i]->getModel();
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
			post_processing_shader.setUint("post_processing_type", 0);
			GLAD_CHECK_ERROR(glBindVertexArray(screen_quad_VAO));
			GLAD_CHECK_ERROR(glDisable(GL_DEPTH_TEST));
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, scene_gui.getFBO()));
			GLAD_CHECK_ERROR(glBindTexture(GL_TEXTURE_2D, screen_quad_texture));
			GLAD_CHECK_ERROR(glDrawArrays(GL_TRIANGLES, 0, 6));
			GLAD_CHECK_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, 0));

			// GUI
			editor_gui.update();
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

#ifdef IMGUI_LAYER
			context::renderImgui();
#endif
			context::window::swapBuffers();
			context::window::pollEvents();
			update(delta_time.getSeconds(), camera, direction, should_isolte);
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}
