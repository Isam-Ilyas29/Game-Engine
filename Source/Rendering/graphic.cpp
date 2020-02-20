#include "graphic.h"

#include <GLFW/glfw3.h>



/*----------------------------------------------------------------------------------*/

namespace context {

	namespace graphics {

		bool initialiseGraphics() {
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
				std::cerr << "Failed to initialize GLAD" << std::endl;
				return false;
			}
			glEnable(GL_DEPTH_TEST);
			return true;
		}
	}
}


// VERTEX

void VertexData::setVBO(unsigned int VBO) {
	mVBO = VBO;
}
void VertexData::setVAO(unsigned int VAO) {
	mVAO = VAO;
}
void VertexData::setEBO(unsigned int EBO) {
	mEBO = EBO;
}

VertexData::~VertexData() {
	if (mVBO != 0) {
		glDeleteBuffers(1, &mVBO);
	}
	if (mEBO != 0) {
		glDeleteBuffers(1, &mEBO);
	}
	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
	}
}

void VertexData::bindVBO(const std::vector<float>& vertices, unsigned int VBO) {		// Accepts Vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO);											
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
}
void VertexData::bindVBO(float vertices[], size_t size, unsigned int VBO) {				// Accepts Arrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);									   
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VertexData::bindEBO(const std::vector<unsigned int>& indices, unsigned int EBO) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
}
void VertexData::bindVAO(unsigned int VAO) {
	glBindVertexArray(VAO);
}

// ATTRIBUTES

void VertexData::positionAttrib(size_t stride) {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
}

void VertexData::colourAttrib(size_t stride) {
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void VertexData::textureAttrib(size_t stride) {
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


/*----------------------------------------------------------------------------------*/

Transform::Transform(unsigned int rotation_axis, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: mRotationAxis(rotation_axis), mPosition(position), mTransform(glm::mat4(1.f)), mRotation(rotation), mScale(scale) {

	createModel();
}

void Transform::createModel() {
	auto const originalMat = glm::mat4(1.0f);

	auto const scale = glm::scale(originalMat, mScale);
	auto const translate = glm::translate(originalMat, mPosition);
	auto const rotate = glm::toMat4(mRotation);

	mTransform = translate * rotate * scale;
}

const glm::mat4 Transform::getModel() const {
	return mTransform;
}

/*----------------------------------------------------------------------------------*/

void screenColour(double r, double g, double b, double a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*----------------------------------------------------------------------------------*/
