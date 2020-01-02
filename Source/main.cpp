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

	glfwSetKeyCallback(window, processInput);


	//Loads all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/*----------------------------------------------------------------------------------*/

	{

		//Build and compile shader program
		std::unique_ptr<Shader> our_shader = std::make_unique<Shader>(environment::ResourcePath("shader.vs").data(), environment::ResourcePath("shader.fs").data());

		//Set up vertex data and configure vertex attributes
		std::vector<float> vertices = {
			//Positions           //Colours           //Texture Coord
			 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,				//Top right
			 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,				//Bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,				//Bottom left
			-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f				//Top left 
		};
		std::vector<unsigned int> indices = {
			0, 1, 3,				//First triangle
			1, 2, 3					//Second triangle
		};

		unsigned int VBO, VAO, EBO;
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &EBO);
		
		{
			//Creates vertex object
			std::unique_ptr<VertexObjects> our_vertex_objects = std::make_unique<VertexObjects>(VAO, VBO, EBO);

			our_vertex_objects->generateVBO(vertices, VBO);
			our_vertex_objects->generateVAO(VAO);
			our_vertex_objects->generateEBO(indices, EBO);

			//Creates attributes object
			std::unique_ptr<Attrib> our_attribs = std::make_unique<Attrib>();

			our_attribs->positionAttrib();
			our_attribs->colourAttrib();
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
				glClear(GL_COLOR_BUFFER_BIT);

				//Bind textures on corresponding texture units
				our_texture1.bind();

				//Binds textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);				//Texture 1
				our_texture1.bind();
				glActiveTexture(GL_TEXTURE1);
				our_texture2.bind();						//Texture 

				//First container
				//---------------------
				glm::mat4 transform = glm::mat4(1.0f);
				transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));							//Position on screen
				transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));		//Position of shape points
				
				unsigned int transform_loc = glGetUniformLocation(our_shader->mID, "transform");
				glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transform));

				//Renders shape
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				//Second transformation
				//---------------------
				transform = glm::mat4(1.0f); // reset it to identity matrix
				transform = glm::translate(transform, glm::vec3(-0.5f, 0.0f, 0.0f));
				float scaleAmount = sin(glfwGetTime());
				transform = glm::scale(transform, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
				glUniformMatrix4fv(transform_loc, 1, GL_FALSE, &transform[0][0]); // this time take the matrix value array's first element as its memory pointer value

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


				//Checks if keys/mouse was pressed or if mouse was moved
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}
	}

	glfwTerminate();
	return 0;
}

