#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "../Camera/camera_property.h"


//Vertex and Attribute Objects
class VertAttribObject {
private:
	unsigned int mVAO, mVBO, mEBO;

public:
	void bindVAO(unsigned int VAO);
	void bindVBO(const std::vector<float>& vertices, unsigned int VBO);		//Accepts Vertices
	void bindVBO(float vertices[], size_t size, unsigned int VBO);			//Accepts Arrays
	void bindEBO(const std::vector<unsigned int>& indices, unsigned int EBO);

	void positionAttrib();
	void colourAttrib();
	void textureCoordAttrib();

	VertAttribObject(const unsigned int& VAO, const unsigned int& VBO, const unsigned int& EBO = 0);
	~VertAttribObject();
};

glm::mat4 getMat4Model(unsigned int i, glm::vec3 cube_positions[]);
