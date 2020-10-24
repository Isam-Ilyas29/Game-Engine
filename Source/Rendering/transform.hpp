#pragma once

#include "Core/std_types.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


class Transform {
private:
	u16 mRotationAxis;

	glm::vec3 mPosition;
	glm::quat mRotation;
	glm::vec3 mScale;

	glm::mat4 mTransform;

public:
	Transform(u16 rotation_axis, glm::vec3 position, glm::quat rotation, glm::vec3 scale = glm::vec3(1.f));
	Transform(const Transform&) = delete;
	Transform& operator=(const Transform&) = delete;

	void createModel();
	const glm::mat4 getModel() const;

	void setRotationAxis(u16 rotation_axis);
	void setPosition(glm::vec3 position);
	void setRotation(glm::quat rotation);
	void setScale(glm::vec3 scale);
};
