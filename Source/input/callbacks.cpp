#include "callbacks.h"



bool PlayerCallback::mMoveForward = false;

int PlayerCallback::registerCallback(std::function<void(GLFWwindow*, int, int, int, int)> callback) {
    int token = 0;
    gCallbacks.emplace(token, callback);
    return token;
}

bool PlayerCallback::unregisterCallback(int token) {
    auto search = gCallbacks.find(token);
    if (search != gCallbacks.end()) {
        gCallbacks.erase(search);
        return true;
    }
    return false;
}

PlayerCallback::PlayerCallback() {
    auto lambda = [this](GLFWwindow* window, int key, int scancode, int action, int mods) { processInput(window, key, scancode, action, mods); };
    mInputToken = registerCallback(lambda);
}

PlayerCallback::~PlayerCallback(){
    unregisterCallback(mInputToken);
}

bool PlayerCallback::getMoveForward(){
    return mMoveForward;
}

void PlayerCallback::processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        switch (action) {

        case (GLFW_PRESS):
            mMoveForward = true;

        case (GLFW_RELEASE):
            mMoveForward = false;
        }
    }
}


