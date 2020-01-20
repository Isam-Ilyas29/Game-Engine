#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framesPerSecond();

class Camera {
public:
	void perFrameTimeLogic();
	glm::mat4 getMat4Projection();
	glm::mat4 getMat4View();
	glm::mat4 getMat4Transform();
};

