#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <assert.h>

#include "rendering/shader.h"
#include "environment/environment.h"
#include "rendering/graphics.h"
#include "rendering/texture.h"
#include "window.h"
#include "input/keyboard.h"


int main(int argc, char* argv[]) {

	//Fixes path issues
#if defined(_WIN32) || defined(_WIN64)
	const char PATH_DELIM = '\\';
#else
	const char PATH_DELIM = '/';
#endif

	std::string exeFile = argv[0];
	environment::exeDirectory = exeFile.substr(0, exeFile.find_last_of(PATH_DELIM));
	environment::resourcesPath = environment::exeDirectory + "/../Resources";

	/*----------------------------------------------------------------------------------*/

	//Initialises 'glfw core 3.3'
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFW window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Game", NULL, NULL);
	windowCreation(window);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetKeyCallback(window, processInput);


	//Loads all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	/*----------------------------------------------------------------------------------*/

	{

		//Build and compile shader program
		std::unique_ptr<Shader> our_shader = std::make_unique<Shader>(environment::ResourcePath("shader.vs").data(), environment::ResourcePath("shader.fs").data());

		//Set up vertex data and configure vertex attributes
		std::vector<float> vertices = {
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

		std::vector<glm::vec3> cube_positions = {
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
			//Creates vertex object
			std::unique_ptr<VertexObjects> our_vertex_objects = std::make_unique<VertexObjects>(VAO, VBO);

			our_vertex_objects->generateVBO(vertices, VBO);
			our_vertex_objects->generateVAO(VAO);

			//Creates attributes object
			std::unique_ptr<Attrib> our_attribs = std::make_unique<Attrib>();

			our_attribs->positionAttrib();
			our_attribs->textureCoordAttrib();

			/*----------------------------------------------------------------------------------*/

			int width, height, nrChannels;

			//Tells stb_image to flip loaded texture's on the y-axis.
			stbi_set_flip_vertically_on_load(true);

			//TEXTURE 1
			unsigned char* data = stbi_load(environment::ResourcePath("Textures/MetalTexture1.jpg").data(), &width, &height, &nrChannels, 0);

			Texture our_texture1(data, width, height, 1);

			stbi_image_free(data);

			//TEXTURE 2
			data = stbi_load(environment::ResourcePath("Textures/GraffitiTexture1.png").data(), &width, &height, &nrChannels, 0);

			Texture our_texture2(data, width, height, 2);

			stbi_image_free(data);

			/*----------------------------------------------------------------------------------*/

			our_shader->use();
			our_shader->setInt("texture2", 1);

			/*----------------------------------------------------------------------------------*/

			//Render loop
			while (!glfwWindowShouldClose(window)) {

				//Polygonmode variable is set to false, so user can toggle it on -as they desire
				PolygonToggle pt;
				pt.polygon_mode = false;
				pt.toggle(window);
				glPolygonMode(GL_FRONT_AND_BACK, pt.polygon_mode ? GL_LINE : GL_FILL);

				//Renders Screen Colour
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//Bind textures on corresponding texture units
				our_texture1.bind();

				//Binds textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);				//Texture 1
				our_texture1.bind();
				glActiveTexture(GL_TEXTURE1);
				our_texture2.bind();						//Texture 2

				//Transformations
				glm::mat4 view = glm::mat4(1.0f);
				glm::mat4 projection = glm::mat4(1.0f);
				glm::mat4 transform = glm::mat4(1.0f);

				view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
				projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
				transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));						
				transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));	

				our_shader->setMat4("projection", projection);
				our_shader->setMat4("view", view);

				unsigned int transform_loc = glGetUniformLocation(our_shader->mID, "transform");
				glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

				//Render boxes
				glBindVertexArray(VAO);
				for (unsigned int i = 0; i < 10; i++)
				{
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, cube_positions[i]);
					float angle = 20.0f * i;
					model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
					our_shader->setMat4("model", model);

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}

				//Checks if keys/mouse was pressed or if mouse was moved
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}
	}

	glfwTerminate();
	return 0;
}