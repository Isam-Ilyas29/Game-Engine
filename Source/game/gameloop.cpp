#include "gameloop.h"

#include "../Context/context.h"
#include "../Rendering/graphic.h"
#include "../Rendering/shader.h"
#include "../Rendering/texture.h"
#include "../Input/input_responder.h"
#include "../Tools/tool.h"
#include "../Rendering/ImGUI/editor.h"



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

		Texture error_texture(environment::ResourcePath("Textures/error_texture1.png"));

		std::vector<std::unique_ptr<Texture>> loaded_textures;

		loaded_textures.push_back(NULL);
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

#ifdef DEBUG_MODE
			// Render imGUI
			context::createImguiWindow("My GUI###GUI1");

			if (ImGui::BeginTabBar("###tab_bar1")) {

				if (ImGui::BeginTabItem("Editor###editor1")) {
					ImGui::TextWrapped("\n");

					collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, true);
					collapsingHeader::colour(true);
					collapsingHeader::miscellaneous(true);

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Help###help1")) {
					collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, false);
					collapsingHeader::colour(false);
					collapsingHeader::miscellaneous(false);

					ImGui::TextWrapped("\n");

					collapsingHeader::controlsText(true);
					collapsingHeader::aboutText(true);

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
			else {
				collapsingHeader::texture(textures, std::move(loaded_textures), error_texture, transparent1, true);
				collapsingHeader::colour(true);
				collapsingHeader::miscellaneous(true);
			}

			ImGui::End();
#endif

			// Projection + View + Transform
			glm::mat4 projection = camera.getMat4Projection();
			shader.setMat4("projection", projection);

			glm::mat4 view = camera.getMat4View();
			shader.setMat4("view", view);

			glm::mat4 transform = camera.getMat4Transform();
			unsigned int transform_loc = glGetUniformLocation(shader.mID, "transform");
			shader.modMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

			// Renders boxes
			for (unsigned int i = 0; i < 10; i++) {
				glm::mat4 model = getMat4Model(i, cube_positions);

				shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			// Unbind all texture units 
			std::vector<unsigned int> texture_units = { 0, 1, 2 };
			Texture::unbind(texture_units);

			glBindVertexArray(VAO);


#ifdef DEBUG_MODE
			context::renderImgui();
#endif
			context::window::swapBuffers();
			context::window::pollEvents();
			update(delta_time, camera);
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}

