#include "Rendering/transform.hpp"


Transform::Transform(u16 rotation_axis, glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: mRotationAxis(rotation_axis), mPosition(position), mRotation(rotation), mScale(scale), mTransform(glm::mat4(1.f)) {

	createModel();
}

void Transform::createModel() {
	const auto originalMat = glm::mat4(1.f);
		 
	const auto scale = glm::scale(originalMat, mScale);
	const auto translate = glm::translate(originalMat, mPosition);
	const auto rotate = glm::toMat4(mRotation);

	mTransform = translate * rotate * scale;
}

const glm::mat4 Transform::getModel() const {
	return mTransform;
}

void Transform::setRotationAxis(u16 rotation_axis) {
	mRotationAxis = rotation_axis;
}
void Transform::setPosition(glm::vec3 position) {
	mPosition = position;
}
void Transform::setRotation(glm::quat rotation) {
	mRotation = rotation;
}
void Transform::setScale(glm::vec3 scale) {
	mScale = scale;
}
