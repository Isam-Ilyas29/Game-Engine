#include "callback.h"



bool PlayerCallback::mMoveForward = false;

PlayerCallback::PlayerCallback() {
    auto lambda = [this](int key, int action) { processInput(key, action); };
    mInputToken = input::registerCallback(lambda);
}

PlayerCallback::~PlayerCallback() {
    input::unregisterCallback(mInputToken);
}

void PlayerCallback::moveForwardLogic() {
    /*if (mMoveForward) {
        std::cout << "MOVE FORWARD!" << std::endl;
    }
    else {
        std::cout << "STATIONARY!" << std::endl;
    }*/
}

void PlayerCallback::processInput(int key, int action) {
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

