#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>

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
