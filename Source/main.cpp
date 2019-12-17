#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <iostream>

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static void GlCheckError() {
	while (GLenum error = glGetError()) {
		std::cerr << "[OpenGL Error] (" << error << ")" << std::endl;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);

//Screen size settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char* argv[]) {

	//Initialises 'glfw core 3.3'
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//GLFW window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Game", NULL, NULL);
	if (window == NULL) {
		//Window error message
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Loads all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		//GLAD error message
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Build and compile shader program
	Shader our_shader("shader.fs", "shader.vs");

	//Set up vertex data and configure vertex attributes
	float vertices[] = {
		//Positions         //Colours
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  //Bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  //Bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   //Top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO); //Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Draws outlines 'glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);'

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		//Checks for input
		process_input(window);

		//Renders Screen Colour
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Renders shape
		our_shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Checks if keys/mouse was pressed or if mouse was moved
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	//De-allocate GLFW resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//After leaving the loop we would need to terminate window
	glfwTerminate();
	return 0;
}

//Process all input
void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	glfwSetWindowShouldClose(window, true);
}

//[GLFW] Whenever the window is resized (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//Makes sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}