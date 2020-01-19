#include "callback.h"


bool PlayerCallback::mMoveForward = false;

PlayerCallback::PlayerCallback() {
    auto lambda = [this](int key, int action) { processInput(key, action); };
    mInputToken = input::registerCallback(lambda);
}

PlayerCallback::~PlayerCallback() {
    input::unregisterCallback(mInputToken);
}

void PlayerCallback::update(float delta_time) {
    float time_between_fps_display = 2.0f;

    time_between_fps_display -= delta_time;
    if (time_between_fps_display <= 0.0f) {
        time_between_fps_display = 2.0f;
        Camera::sFramesPerSecond();
    }

    //MOVE FORWARD ['W']
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

