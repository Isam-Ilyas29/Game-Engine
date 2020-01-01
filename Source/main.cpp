#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <assert.h>

#include "shader.h"
#include "environment/environment.h"
#include "graphics.h"
#include "texture.h"

//Screen size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

bool polygon_mode;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void polygonToggle(GLFWwindow* window);

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
	if (window == NULL) {
		//Window error message:
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//
	glfwSetKeyCallback(window, processInput);


	//Loads all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//GLAD error message:
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	{
		/*----------------------------------------------------------------------------------*/

		//Build and compile shader program
		std::unique_ptr<Shader> our_shader = std::make_unique<Shader>(environment::ResourcePath("shader.vs").data(), environment::ResourcePath("shader.fs").data());

		//Set up vertex data and configure vertex attributes
		std::vector<float> vertices = {
			//Positions           //Colours           //Texture Coord
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,				//Top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,				//Bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,				//Bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f				//Top left 
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

			//TEXTURE 1
			unsigned char* data = stbi_load(environment::ResourcePath("Textures/MetalTexture1.jpg").data(), &width, &height, &nrChannels, 0);

			std::unique_ptr<Texture> our_texture1 = std::make_unique<Texture>(data, width, height, 1);

			our_texture1->getID();

			//TEXTURE 2
			data = stbi_load(environment::ResourcePath("Textures/GraffitiTexture1.png").data(), &width, &height, &nrChannels, 0);

			std::unique_ptr<Texture> our_texture2 = std::make_unique<Texture>(data, width, height, 2);

			our_texture2->getID();

			/*----------------------------------------------------------------------------------*/

			our_shader->use();
			our_shader->setInt("texture2", 1);

			/*----------------------------------------------------------------------------------*/

			//Render loop
			while (!glfwWindowShouldClose(window)) {

				//Polygonmode variable is set to false, so user can toggle it on -as they desire
				polygon_mode = false;
				//Calls function to check if user wants to toggle 'glPolygonMode'
				polygonToggle(window);
				//Allows user to toggle 'glPolygonMode' on and off
				glPolygonMode(GL_FRONT_AND_BACK, polygon_mode ? GL_LINE : GL_FILL);

				//Renders Screen Colour
				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);

				//Bind textures on corresponding texture units
				our_texture1->bind();

				//Binds textures on corresponding texture units
				glActiveTexture(GL_TEXTURE0);				//Texture 1
				our_texture1->bind();
				glActiveTexture(GL_TEXTURE1);
				our_texture2->bind();	   //Texture 2

				//Renders shape
				our_shader->use();
				glBindVertexArray(VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				//Checks if keys/mouse was pressed or if mouse was moved
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
		}
	}

	//After leaving the loop we would need to terminate window
	glfwTerminate();
	return 0;
}

/*----------------------------------------------------------------------------------*/

void polygonToggle(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		polygon_mode = !polygon_mode;
	}
}

//Process all input
void processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {

		//'Esc'
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS) {
			//PRESS = Close window
			glfwSetWindowShouldClose(window, true);
		}
		else if (action == GLFW_RELEASE) {
			//RELEASE = Pass
			break;
		}
		else if (action == GLFW_REPEAT) {
			//HELD_DOWN = Pass
			break;
		}
		break;
	}
}

//[GLFW] Whenever the window is resized (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	//Makes sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}