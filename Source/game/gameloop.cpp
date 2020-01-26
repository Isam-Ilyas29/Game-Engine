#include "gameloop.h"

#include "../Context/context.h"
#include "../Rendering/graphic.h"
#include "../Rendering/shader.h"
#include "../Rendering/texture.h"
#include "../Input/input_respondant.h"



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

		// Avoid cursor jump
		input::setCursorPos();

		mouseProp(true, -90.0f, 0.0f, (800.0f / 2.0), (600.0 / 2.0), 45.0f);

		float time = 2.0f;

		/*----------------------------------------------------------------------------------*/

		Camera camera;

		/*----------------------------------------------------------------------------------*/

		// Game loop
		while (context::window::isClosed(context::window::getWindow()) == false) {

			update(delta_time);

			camera.perFrameTimeLogic();

			// Displays FPS every 2 seconds
			time -= delta_time;
			if (time <= 0.0f) {
				time = 2.0f;
				framesPerSecond();
			}

			screenColour(0.2f, 0.3f, 0.3f, 1.0f);

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

			context::window::swapBuffers();
			input::startFrame();
			context::window::pollEvents();
		}
	}

	glfwTerminate();
	return true;
}

