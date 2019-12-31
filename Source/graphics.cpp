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
	glDeleteVertexArrays(1, &rVAO);
	glDeleteBuffers(1, &rVBO);
	glDeleteBuffers(1, &rEBO);
}


void VertexObjects::generateVBO(const std::vector<float>& vertices, unsigned int VBO) {
	//Binds and sets Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
}

void VertexObjects::generateVAO(unsigned int VAO) {
	//Binds the Vertex Array Object
	glBindVertexArray(VAO);	//Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
}

void VertexObjects::generateEBO(const std::vector<unsigned int>& indices, unsigned int EBO) {
	//Binds EBO, it stores indices that OpenGL uses to decide what vertices to draw
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
}

//ATTRIBUTES
/*----------------------------------------------------------------------------------*/

void Attrib::positionAttrib() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Attrib::colourAttrib() {
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Attrib::textureCoordAttrib() {
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

/*----------------------------------------------------------------------------------*/

//TODO: Textures

