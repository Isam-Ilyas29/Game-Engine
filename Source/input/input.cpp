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

    void startFrame() {
        state.released.clear();
    }
    void onInput(int key, int action) {
        switch (action) {

        case GLFW_REPEAT:
        case GLFW_PRESS:
            state.released.erase(key);

            if (isPressed(key)) {
                state.pressed.erase(key);
                state.held.insert(key);
            }
            else {
                state.pressed.insert(key);
            }

            break;

        case GLFW_RELEASE:
            state.held.erase(key);
            state.pressed.erase(key);

            state.released.insert(key);

            break;

        default:
            break;
        }
    }
    State const& getState() {  
        return state;
    }

    bool isPressed(int key) { 
        return state.pressed.find(key) != state.pressed.end();
    }
    bool isHeld(int key) {  
        return state.held.find(key) != state.held.end();
    }
    bool isReleased(int key) {  
        return state.released.find(key) != state.released.end();
    }
}

