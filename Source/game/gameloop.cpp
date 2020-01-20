#include "gameloop.h"

#include "../context/context.h"
#include "../rendering/graphic.h"
#include "../rendering/shader.h"
#include "../rendering/texture.h"
#include "../input/callback.h"
#include "../window/window.h"



bool gameloop::run(int argc, char* argv[]) {

	//Fixes path issues
#if defined(_WIN32) || defined(_WIN64)
	const char PATH_DELIM = '\\';
#else
	const char PATH_DELIM = '/';
#endif

	std::filesystem::path exeFile = argv[0];
	environment::exeDirectory = exeFile.parent_path();
	environment::resourcesPath = environment::exeDirectory.parent_path() / "Resources";

	/*----------------------------------------------------------------------------------*/

	initialiseGLFW();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Game", NULL, NULL);
	bool success  = setupWindow(window);
	if (!success) {
		return false;
	}

	setupContext(window);

	success = initialiseGlad();
	if (!success) {
		return false;
	}

	/*----------------------------------------------------------------------------------*/

	{

		//Build and compile shader program
		std::unique_ptr<Shader> our_shader = std::make_unique<Shader>(environment::ResourcePath("shader.vs").string().c_str(), environment::ResourcePath("shader.fs").string().c_str());

		//Set up vertex data and configure vertex attributes
		float vertices[] = {
			//Positions							   
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

		unsigned int VBO, VAO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);

		{
			//Creates vertex/attribute object
			std::unique_ptr<VertAttribObject> vertex_and_attrib_object = std::make_unique<VertAttribObject>(VAO, VBO);

			vertex_and_attrib_object->bindVBO(vertices, sizeof(vertices), VBO);
			vertex_and_attrib_object->bindVAO(VAO);

			vertex_and_attrib_object->positionAttrib();
			vertex_and_attrib_object->textureCoordAttrib();

			/*----------------------------------------------------------------------------------*/

			//TEXTURE 1
			Texture our_texture1(environment::ResourcePath("Textures/MetalTexture1.jpg"));
			//TEXTURE 2
			Texture our_texture2(environment::ResourcePath("Textures/GraffitiTexture1.png"));

			/*----------------------------------------------------------------------------------*/

			our_shader->use();
			our_shader->setInt("texture2", 1); //Comment out to remove graffiti

			//Avoid cursor jump
			glfwSetCursorPos(window, lastX, lastY);

			mouseProp(true, -90.0f, 0.0f, (800.0f / 2.0), (600.0 / 2.0), 45.0f);

			float time = 2.0f;

			//Input object
			std::unique_ptr<PlayerCallback> test = std::make_unique<PlayerCallback>();

			/*----------------------------------------------------------------------------------*/

			//Game loop
			while (!glfwWindowShouldClose(window)) {

				test->update(delta_time);

				//Creates camera object
				Camera camera;

				camera.perFrameTimeLogic();

				//Displays FPS every 2 seconds
				time -= delta_time;
				if (time <= 0.0f) {
					time = 2.0f;
					framesPerSecond();
				}

				//Renders Screen Colour
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//Binds textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);
				our_texture1.bind();
				glActiveTexture(GL_TEXTURE1);
				our_texture2.bind();

				//Projection + View + Transform
				glm::mat4 projection = camera.getMat4Projection();
				our_shader->setMat4("projection", projection);

				glm::mat4 view = camera.getMat4View();
				our_shader->setMat4("view", view);

				glm::mat4 transform = camera.getMat4Transform();
				unsigned int transform_loc = glGetUniformLocation(our_shader->mID, "transform");
				glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

				//Render boxes
				glBindVertexArray(VAO);
				for (unsigned int i = 0; i < 10; i++) {
					glm::mat4 model = getMat4Model(i, cube_positions);

					our_shader->setMat4("model", model);

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}

				//Checks if keys/mouse was pressed or if mouse was moved
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}
	}
	return true;
	glfwTerminate();
}

