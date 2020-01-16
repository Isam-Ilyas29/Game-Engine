#include "callbacks.h"



namespace
{
    std::unordered_map<input::Token, input::Callback> gCallbacks;
}

input::Token input::registerCallback(input::Callback callback) {
    int token = 0;
    gCallbacks.emplace(token, callback);

    std::cout << "'registerCallback' called!" << std::endl;

    return token;
}
bool input::unregisterCallback(input::Token token) {
    auto search = gCallbacks.find(token);
    if (search != gCallbacks.end()) {
        gCallbacks.erase(search);

        std::cout << "'unregisterCallback' called!" << std::endl;

        return true;
    }

    std::cout << "'unregisterCallback' called!" << std::endl;

    return false;
}

/*----------------------------------------------------------------------------------*/

bool PlayerCallback::mMoveForward = false;

PlayerCallback::PlayerCallback() {
    auto lambda = [this](GLFWwindow* window, int key, int scancode, int action, int mods) { processInput(window, key, scancode, action, mods); };
    mInputToken = input::registerCallback(lambda);

    std::cout << "Constructed" << std::endl;
}

PlayerCallback::~PlayerCallback() {
    input::unregisterCallback(mInputToken);

    std::cout << "Destructed" << std::endl;
}

void PlayerCallback::moveForwardLogic() {
   /* if (mMoveForward) {
        std::cout << "MOVE FORWARD!" << std::endl;
    }
    else {
        std::cout << "STATIONARY!" << std::endl;
    } */
}

void PlayerCallback::processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W) {
        switch (action) {

        case (GLFW_PRESS):
            std::cout << "'W' pressed." << std::endl;

            mMoveForward = true;

        case (GLFW_RELEASE):
            std::cout << "'W' released." << std::endl;

            mMoveForward = false;
        }
    }
}


