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
