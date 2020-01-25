#pragma once

#include <iostream>
#include <functional>
#include <map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.h"
#include "../Camera/camera.h"


class inputRespondant final {
    void update(float delta_time);
};




