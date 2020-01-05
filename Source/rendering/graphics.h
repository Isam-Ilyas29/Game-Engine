#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "../camera/camera_properties.h"

class Attrib{
public:
	void positionAttrib();
	void colourAttrib();
	void textureCoordAttrib();
};

class VertexObjects {
private:
	unsigned int rVAO, rVBO, rEBO;

public:
	void generateVAO(unsigned int VAO);
	void generateVBO(const std::vector<float>& vertices, unsigned int VBO);
	void generateEBO(const std::vector<unsigned int>& indices, unsigned int EBO);

	VertexObjects(const unsigned int& VAO, const unsigned int& VBO, const unsigned int& EBO = 0);
	~VertexObjects();
};

glm::mat4 getMat4Model(unsigned int i, std::vector<glm::vec3>& cube_positions);
