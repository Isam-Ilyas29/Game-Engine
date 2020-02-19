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
/*----------------------------------------------------------------------------------*/

VertAttribObject::VertAttribObject(unsigned int VAO, unsigned int VBO, unsigned int EBO) {
	mVAO = VAO;
	mVBO = VBO;
	mEBO = EBO;
}

VertAttribObject::~VertAttribObject() {
	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
	}
	if (mEBO != 0) {
		glDeleteVertexArrays(1, &mEBO);
	}
	if (mEBO != 0) {
		glDeleteVertexArrays(1, &mEBO);
	}
}

void VertAttribObject::bindVBO(const std::vector<float>& vertices, unsigned int VBO) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);
}
void VertAttribObject::bindVBO(float vertices[], size_t size, unsigned int VBO) {
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
void VertAttribObject::bindVAO(unsigned int VAO) {
	glBindVertexArray(VAO);
}
void VertAttribObject::bindEBO(const std::vector<unsigned int>& indices, unsigned int EBO) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW);
}

// ATTRIBUTES
/*----------------------------------------------------------------------------------*/

void VertAttribObject::positionAttrib() {
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void VertAttribObject::colourAttrib() {
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void VertAttribObject::textureCoordAttrib() {
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


/*----------------------------------------------------------------------------------*/

Transform::Transform(unsigned int rotation_axis, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: mRotationAxis(rotation_axis), mPosition(position), mTransform(glm::mat4(1.f)), mRotation(rotation), mScale(scale) {

	createModel();
}

void Transform::createModel() {
	mTransform = glm::translate(mTransform, mPosition);
	mTransform = mTransform * glm::toMat4(mRotation);
	mTransform = glm::scale(mTransform, mScale);
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
