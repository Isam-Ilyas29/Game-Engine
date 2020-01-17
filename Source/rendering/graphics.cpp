#include "graphics.h"


//VERTEX OBJECT
/*----------------------------------------------------------------------------------*/

VertexObjects::VertexObjects(const unsigned int& VAO, const unsigned int& VBO, const unsigned int& EBO) {
	rVAO = VAO;
	rVBO = VBO;
	rEBO = EBO;
}

VertexObjects::~VertexObjects() {
	//De-allocate GLFW resources
	if (rVAO != 0) {
		glDeleteVertexArrays(1, &rVAO);
	}
	if (rEBO != 0) {
		glDeleteVertexArrays(1, &rEBO);
	}
	if (rEBO != 0) {
		glDeleteVertexArrays(1, &rEBO);
	}
}


void VertexObjects::generateVBO(const std::vector<float>& vertices, unsigned int VBO) {
	//Binds and sets Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
}

void VertexObjects::generateVAO(unsigned int VAO) {
	//Binds the Vertex Array Object
	glBindVertexArray(VAO);
}

void VertexObjects::generateEBO(const std::vector<unsigned int>& indices, unsigned int EBO) {
	//Binds EBO, it stores indices that OpenGL uses to decide what vertices to draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
}

//ATTRIBUTES
/*----------------------------------------------------------------------------------*/

void Attrib::positionAttrib() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Attrib::colourAttrib() {
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Attrib::textureCoordAttrib() {
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

/*----------------------------------------------------------------------------------*/

glm::mat4 getMat4Model(unsigned int i, std::vector<glm::vec3>& cube_positions) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, cube_positions[i]);
	float angle = 20.0f * i;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

	return model;
}
