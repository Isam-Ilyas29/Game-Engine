#include "input.h"

#include "../Context/context.h"
#include "../Camera/camera_property.h"

#include <GLFW/glfw3.h>


namespace {

    input::State state;
}

namespace input {

    void setCursorPos() {
        glfwSetCursorPos(context::window::getWindow(), lastX, lastY);
    }

    /*----------------------------------------------------------------------------------*/

    void startFrame() {    }
    void onInput(int key, int action) {    }
    State const& getState() {    }

    bool isPressed(int key) {    }
    bool isHeld(int key) {    }
    bool isReleased(int key) {    }
}

