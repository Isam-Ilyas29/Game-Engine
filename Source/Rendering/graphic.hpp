#pragma once

#include "Core/std_types.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <vector>
#include <filesystem>
#include <array>


/*----------------------------------------------------------------------------------*/

// Context for graphics (glad)

namespace context {
	namespace graphics {
		bool initialiseGraphics();
	}
}

/*----------------------------------------------------------------------------------*/

// Error handling

#define GLAD_CHECK_ERROR(expression)			\
	do {										\
		expression;								\
		gladCheckError(__FILE__, __LINE__);		\
	} while (0);								


void gladCheckError(std::filesystem::path file, u32 line_number);

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

	void bindVBO(const std::vector<f32>& vertices, unsigned int VBO);					// Accepts Vectors
	void bindVBO(f32 vertices[], size_t size, unsigned int VBO);			// Accepts Arrays
	void bindEBO(const std::vector<u8>& indices, unsigned int EBO);
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

	u16 mRotationAxis;

public:
	Transform(u16 rotation_axis, glm::vec3 positions, glm::quat rotation, glm::vec3 scale = glm::vec3(1.f));
	Transform(const Transform&) = delete;
	Transform& operator=(const Transform&) = delete;

	void createModel();
	const glm::mat4 getModel() const;
};

/*----------------------------------------------------------------------------------*/

// Set background colour

void setBackgroundColour(f32 r, f32 g, f32 b, f32 a);

/*----------------------------------------------------------------------------------*/
