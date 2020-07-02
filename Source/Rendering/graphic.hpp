#pragma once

#include "Core/std_types.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include <filesystem>


/*----------------------------------------------------------------------------------*/

// Context for graphics (glad)

namespace context {
	namespace graphics {
		bool initialiseGraphics();

	} // namespace graphics

} // namespace context

/*----------------------------------------------------------------------------------*/

// Error handling

#define GLAD_CHECK_ERROR(expression)			\
	do {										\
		expression;								\
		gladCheckError(__FILE__, __LINE__);		\
	} while (0);								


void gladCheckError(std::filesystem::path file, u32 line_number);

/*----------------------------------------------------------------------------------*/

// Transform class

class Transform {
private:
	u16 mRotationAxis;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::mat4 mTransform;

public:
	Transform(u16 rotation_axis, glm::vec3 positions, glm::quat rotation, glm::vec3 scale = glm::vec3(1.f));
	Transform(const Transform&) = delete;
	Transform& operator=(const Transform&) = delete;

	void createModel();
	const glm::mat4 getModel() const;
};

/*----------------------------------------------------------------------------------*/
