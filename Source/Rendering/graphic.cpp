#include "Rendering/graphic.hpp"

#include "Core/logger.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include <string>


/*----------------------------------------------------------------------------------*/

// Context for graphics

bool context::graphics::initialiseGraphics() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		log(logType::ERROR, "CONTEXT | Failed to initialise GLAD");
		return false;
	}
	GLAD_CHECK_ERROR(glEnable(GL_DEPTH_TEST));

	log(logType::INFO, "CONTEXT | glad Successfully Initialised");
	return true;
}

/*----------------------------------------------------------------------------------*/

// Error handling

void gladCheckError(std::filesystem::path file, u32 line_number) {
	std::string error_type;
	std::string error_description;

	GLenum error_code = glad_glGetError(); 
	switch (error_code) {
	case GL_INVALID_ENUM:
		error_type = "GL_INVALID_ENUM";
		error_description = "Enumeration parameter is not a legal enumeration for function call";
		break;
	case GL_INVALID_VALUE:
		error_type = "GL_INVALID_VALUE";
		error_description = "Numeric parameter is not a legal value for function call";
		break;
	case GL_INVALID_OPERATION:
		error_type = "GL_INVALID_OPERATION";
		error_description = "Specified operation is not allowed in current state";
		break;
	case GL_STACK_OVERFLOW:
		error_type = "GL_STACK_OVERFLOW";
		error_description = "Specified operation cannot be done as stack is at mazimum capacity";
		break;
	case GL_STACK_UNDERFLOW:
		error_type = "GL_STACK_UNDERFLOW";
		error_description = "Specified operation cannot be done as stack is at minimum capacity";
		break;
	case GL_OUT_OF_MEMORY:
		error_type = "GL_OUT_OF_MEMORY";
		error_description = "Specified operation cannot be done as memory cannot be allocated";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		error_type = "GL_INVALID_FRAMEBUFFER_OPERATION";
		error_description = "Cannot use framebuffer as it is not complete";
		break;
	case GL_CONTEXT_LOST:
		error_type = "GL_CONTEXT_LOST";
		error_description = "OpenGL context has been lost, due to a graphics card reset";
		break;
	}

	if (error_code != GL_NO_ERROR) {
		log(logType::ERROR, fmt::format("GLAD ERROR | {} | {} : {} | \"{}\", \"Line. {}\"", error_code, error_type, error_description, file.generic_string(), line_number));
	}
}

/*----------------------------------------------------------------------------------*/

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
		GLAD_CHECK_ERROR(glDeleteBuffers(1, &mVBO));
	}
	if (mEBO != 0) {
		GLAD_CHECK_ERROR(glDeleteBuffers(1, &mEBO));
	}
	if (mVAO != 0) {
		GLAD_CHECK_ERROR(glDeleteVertexArrays(1, &mVAO));
	}
}

void VertexData::bindVBO(const std::vector<f32>& vertices, unsigned int VBO) {		    // Accepts Vectors
	GLAD_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, VBO));											
	GLAD_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW));
}
void VertexData::bindVBO(f32 vertices[], size_t size, unsigned int VBO) {				// Accepts Arrays
	GLAD_CHECK_ERROR(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLAD_CHECK_ERROR(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}
void VertexData::bindEBO(const std::vector<u8>& indices, unsigned int EBO) {
	GLAD_CHECK_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLAD_CHECK_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices.front(), GL_STATIC_DRAW));
}
void VertexData::bindVAO(unsigned int VAO) {
	GLAD_CHECK_ERROR(glBindVertexArray(VAO));
}

// ATTRIBUTES

void VertexData::positionAttrib(unsigned int location, size_t stride) {
	GLAD_CHECK_ERROR(glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (void*)0));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(location));
}

void VertexData::colourAttrib(unsigned int location, size_t stride) {
	GLAD_CHECK_ERROR(glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(location));
}

void VertexData::textureAttrib(unsigned int location, size_t stride) {
	GLAD_CHECK_ERROR(glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))));
	GLAD_CHECK_ERROR(glEnableVertexAttribArray(location));
}


/*----------------------------------------------------------------------------------*/

// Transform class

Transform::Transform(u16 rotation_axis, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: mRotationAxis(rotation_axis), mPosition(position), mRotation(rotation), mScale(scale), mTransform(glm::mat4(1.f)) {

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
