#pragma once

#include <iostream>
#include <functional>
#include <map>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.h"
#include "../Camera/camera.h"


class PlayerCallback final {
private:
    input::Token mInputToken;

public:
    PlayerCallback();
    ~PlayerCallback();

    void update(float delta_time);
};




