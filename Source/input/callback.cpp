#include "callback.h"



PlayerCallback::PlayerCallback() {
    auto lambda = [this](int key, int action) { input::onInput(key, action); };
    mInputToken = input::registerCallback(lambda);
}

PlayerCallback::~PlayerCallback() {
    input::unregisterCallback(mInputToken);
}

void PlayerCallback::update(float delta_time) {
    //MOVE FORWARD ['W']

    /*
    if (mMoveForward) {
        std::cout << "MOVE FORWARD!" << std::endl;
    }
    else {
        std::cout << "STATIONARY!" << std::endl;
    }
    */
}

