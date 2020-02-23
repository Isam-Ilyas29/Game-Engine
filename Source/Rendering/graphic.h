#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <vector>



/*----------------------------------------------------------------------------------*/

// Context for graphics (glad)

namespace context {
	namespace graphics {
		bool initialiseGraphics();
	}
}

/*----------------------------------------------------------------------------------*/

// Vertex and Attribute class

class VertexData final {
private:
	unsigned int mVAO, mVBO, mEBO = 0;

public:
	void setVBO(unsigned int VBO);
	void setEBO(unsigned int EBO);
	void setVAO(unsigned int VAO);

	~VertexData();

	void bindVBO(const std::vector<float>& vertices, unsigned int VBO);		// Accepts Vertices
	void bindVBO(float vertices[], size_t size, unsigned int VBO);			// Accepts Arrays
	void bindEBO(const std::vector<unsigned int>& indices, unsigned int EBO);
	void bindVAO(unsigned int VAO);

	void positionAttrib(unsigned int location, size_t stride);
	void colourAttrib(unsigned int location, size_t stride);
	void textureAttrib(unsigned int location, size_t stride);
};


/*----------------------------------------------------------------------------------*/

// Transform class

class Transform {
private:
	glm::mat4 mTransform;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	unsigned int mRotationAxis;

public:
	Transform(unsigned int rotation_axis, glm::vec3 positions, glm::quat rotation, glm::vec3 scale = glm::vec3(1.f));
	Transform(const Transform&) = delete;
	Transform& operator=(const Transform&) = delete;

	void createModel();
	const glm::mat4 getModel() const;
};

/*----------------------------------------------------------------------------------*/

// Set background colour

void setBackgroundColour(double r, double g, double b, double a);

/*----------------------------------------------------------------------------------*/

// Error handling

void gladHandleError(const char* name, void* function_pointer, int len_args);

/*----------------------------------------------------------------------------------*/

