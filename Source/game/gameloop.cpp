#include "gameloop.h"

#include "../Context/context.h"
#include "../Rendering/graphic.h"
#include "../Rendering/shader.h"
#include "../Rendering/texture.h"
#include "../Input/input_responder.h"
#include "../Tools/tool.h"

#include <imgui.h>
#include "../Rendering/ImGUI/imgui_impl_glfw.h"
#include "../Rendering/ImGUI/imgui_impl_opengl3.h"



bool gameloop::run(int argc, char* argv[]) {

	std::filesystem::path exeFile = argv[0];
	environment::exeDirectory = exeFile.parent_path();
	environment::resourcesPath = environment::exeDirectory.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	context::initialiseGLFW();

	bool success  = context::window::setupWindow("My Game", 600, 450);
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

		Texture texture1(environment::ResourcePath("Textures/metal_texture1.jpg"));
		Texture texture2(environment::ResourcePath("Textures/graffiti_texture1.png"));

		shader.use();
		shader.setInt("texture2", 1); // Comment out to remove graffiti

		/*----------------------------------------------------------------------------------*/

		float time = 2.0f;

		/*----------------------------------------------------------------------------------*/

		Camera camera(true, -90.0f, 0.0f, (800.0f / 2.0f), (600.0f / 2.0f), 45.0f, 100.0f);

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			camera.perFrameTimeLogic();

			// Displays FPS every 2 seconds
			time -= delta_time;
			if (time <= 0.0f) {
				time = 2.0f;
				std::cout << "FPS: " << framesPerSecond() << std::endl;
			}

			screenColour(0.2, 0.3, 0.3, 1.0);

			// Feed input to imGUI, start new frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			texture1.setTexture(true, 0);
			texture2.setTexture(true, 1);

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

			// Render imGUI
			ImGui::Begin("My GUI");
			ImGui::Button("Hello World!");
			ImGui::End();

			// Render onto screen
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			context::window::swapBuffers();
			context::window::pollEvents();
			update(delta_time, camera);
			input::endFrame();
		}
	}

	glfwTerminate();
	return true;
}

