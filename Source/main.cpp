#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <assert.h>

#include "shader.h"
#include "environment/environment.h"

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
		float vertices[] = {
			//Positions           //Colours           //Texture Coord
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,				//Top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,				//Bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,				//Bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f				//Top left 
		};
		unsigned int indices[] = {
			0, 1, 3,				//First triangle
			1, 2, 3					//Second triangle
		};

		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO); //Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


		//Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		//Colour attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		//Texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		/*----------------------------------------------------------------------------------*/

		unsigned int texture1, texture2;
		int width, height, nrChannels;

		//Tells stb_image to flip loaded texture's on the y-axis.
		stbi_set_flip_vertically_on_load(true);

		//TEXTURE1
		//Loads and creates textures
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Sets the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Sets texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Loads image | Creates texture | Generates mipmaps
		unsigned char* data = stbi_load(environment::ResourcePath("Textures/MetalTexture1.jpg").data(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		//If the texture fails to load
		else {
			//Texture loading error message [1]:
			std::cerr << "Failed to load texture_one" << std::endl;
		}

		//Free's image memory
		stbi_image_free(data);

		//TEXTURE 2
		//Loads and creates textures
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//Sets the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Sets texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Loads image | Creates texture | Generates mipmaps
		data = stbi_load(environment::ResourcePath("Textures/GraffitiTexture1.png").data(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		//If the texture fails to load
		else {
			//Texture loading error message [2]:
			std::cerr << "Failed to load texture_two" << std::endl;
		}

		//Free's image memory
		stbi_image_free(data);

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
			if (polygon_mode == true) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else if (polygon_mode == false) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}

			//Renders Screen Colour
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//Bind textures on corresponding texture units
			glBindTexture(GL_TEXTURE_2D, texture1);

			//Binds textures on corresponding texture units
			glActiveTexture(GL_TEXTURE0);				//Texture 1
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);	   //Texture 2

			//Renders shape
			our_shader->use();
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			//Checks if keys/mouse was pressed or if mouse was moved
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		//De-allocate GLFW resources
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		//De-allocate Textures
		glDeleteTextures(1, &texture1);
		glDeleteTextures(1, &texture2);
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