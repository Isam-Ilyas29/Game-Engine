#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_properties.h"
#include "../rendering/shader.h"
#include "../window.h"

class Camera {
public:
	void perFrameTimeLogic();
	glm::mat4 getMat4Projection();
	glm::mat4 getMat4View();
	glm::mat4 getMat4Transform();
	void framesPerSecond();
};


