#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <set>
#include <unordered_set>

#include "../camera/camera_properties.h"


namespace Input {
    enum StateEnum {
        STATE_FREE,
        STATE_MOVE_FORWARD,
        STATE_MOVE_LEFT,
        STATE_MOVE_BACKWARD,
        STATE_MOVE_RIGHT
    };

    void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);

    struct StateStruct {
        std::unordered_set<int> pressed;
        std::unordered_set<int> held;
        std::unordered_set<int> released;
    };

    void startFrame();
    void onInput(int key, int action);
    StateStruct const& getState();

    bool isPressed(int key);
    bool isHeld(int key);
    bool isReleased(int key);
}
