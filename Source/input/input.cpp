#include "input.h"

#include "../Context/context.h"
#include "../Camera/camera_property.h"


bool mMoveForward = false;

namespace {

    std::unordered_map<input::Token, input::Callback> gCallbacks;
}

namespace input {

    void setCursorPos() {
        glfwSetCursorPos(context::window::getWindow(), lastX, lastY);
    }

    Token registerCallback(Callback callback) {
        int token = 0;
        gCallbacks.emplace(token, callback);
        return token;
    }

    bool unregisterCallback(Token token) {
        auto search = gCallbacks.find(token);
        if (search != gCallbacks.end()) {
            gCallbacks.erase(search);
            return true;
        }
        return false;
    }

    void onInput(int key, int action) {
        if (key == GLFW_KEY_W) {
            switch (action) {

            case (GLFW_PRESS):
                mMoveForward = true;
                break;

            case (GLFW_RELEASE):
                mMoveForward = false;
                break;

            default:
                break;
            }
        }
    }
}

