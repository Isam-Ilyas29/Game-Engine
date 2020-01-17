#pragma once

#include <iostream>
#include <functional>
#include <map>

#include <GLFW/glfw3.h>



namespace input
{
    using Token = int;  // later shared_ptr<int>
    using Callback = std::function<void(GLFWwindow*, int, int, int, int)>;

    Token registerCallback(Callback c);
    bool unregisterCallback(Token token);  // not required with shared_ptr
}

/*----------------------------------------------------------------------------------*/

class PlayerCallback final {
private:
    input::Token mInputToken;
    static bool mMoveForward;

public:
    PlayerCallback();
    ~PlayerCallback();

    void moveForwardLogic();

    static void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
};


/*
class PlayerController final
{
private:
    bool m_bMovingForward = false;

public:
    PlayerController()
    {
        input::register([this](int key, int action) { onInput(key, action); });
    }

    void onInput(int key, int action)
    {
        if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
        {
            m_bMovingForward = action == GLFW_PRESS;
        }
    }
}
*/
/*

//register
input::register([this](int key, int action) { processInput(key, action}; };

input::register(std::function<void(int, int)> callback) {
    Token token = //Makes a new token
    gCallbacks.emplace(token, callback);
    return token;
}

//Process input
void processInput(int key, int action) {
    //Key is press
    //State is triggered
}

if (my_object.jump == true) {
    //jump code
}

//unregister
bool unregister(Token token) {
    auto search = gCallbacks.find(token);
    if (search != gCallbacks.end()) {
        gCallbacks.erase(search);
        return true;
    }
    return false;
}

*/



